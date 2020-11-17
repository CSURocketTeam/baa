/*
 * Sound card modem for Amateur Radio AX25.
 *
 * Copyright (C) Alejandro Santos, 2013, alejolp@gmail.com.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <iostream>
#include <iomanip>
#include <cmath>

#include "audiosource.h"
#include "encoder.h"
#include "extmodem.h"
#include "extconfig.h"

namespace extmodem {

modem::modem() {
}

modem::~modem() {
}

void modem::set_audiosource(audiosource_ptr p) {
	audio_source_ = p;
}

void modem::set_encoder(encoder_ptr p) {
	encoder_ = p;
	p->init(audio_source_.get());
}

void modem::input_callback(audiosource* a, const float* buffer, unsigned long length) {
}

void modem::output_callback(audiosource* a, float* buffer, unsigned long length) {
	(void)a;
	// int channel_count = a->get_out_channel_count();

	if (encoder_.get()) {
		encoder_->output_callback(audio_source_.get(), buffer, length);
	}
}

void modem::start_and_run() {
	audio_source_->set_listener(this);
}

void modem::output_packet_to_sc(frame_ptr fp) {
	encoder_->send(fp);
}


} /* namespace extmodem */
