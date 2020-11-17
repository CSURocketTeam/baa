/*
 * Sound card modem for Amateur Radio AX25.
 *
 * Copyright (C) Alejandro Santos LU4EXT, 2013, alejolp@gmail.com.
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
#include <sstream>
#include <unistd.h>

#include "audiosource_portaudio.h"
#include "audiosource_alsa.h"
#include "audiosource_null.h"
#include "audiosource_loopback.h"
#include "audiosource_wave.h"
#include "extmodem.h"
#include "extconfig.h"

#include "encoder_af1200stj.h"

#include "ax25_utils.h"

#include "frame.h"

#include <boost/smart_ptr.hpp>

using namespace std;
using namespace extmodem;

int main(int argc, char **argv) {
	try {
		config* cfg = config::Instance();

		cfg->init(argc, argv);

		if (cfg->is_help()) {
			return 0;
		}

		if ((cfg->callsign()).empty()) {
			std::cerr << "ERROR: Must provide callsign" << std::endl;
			return 0;
		}

		boost::shared_ptr<modem> em(new modem());
		boost::shared_ptr<audiosource> as;

		if (cfg->audio_backend() == "null") {
			as.reset(new audiosource_null(cfg->sample_rate()));
		} else if (cfg->audio_backend() == "loopback") {
			as.reset(new audiosource_loopback(cfg->sample_rate()));
		} else if (cfg->audio_backend() == "wave") {
			as.reset(new audiosource_wave(cfg->sample_rate()));
		} else if (cfg->audio_backend() == "portaudio") {
#ifdef PORTAUDIO_FOUND
			as.reset(new audiosource_portaudio(cfg->sample_rate()));
#else
			cerr << "ERROR: PORTAUDIO support not compiled" << endl;
			return 1;
#endif
		} else if (cfg->audio_backend() == "alsa") {
#if ALSA_FOUND
			as.reset(new audiosource_alsa(cfg->sample_rate()));
#else
			cerr << "ERROR: ALSA support not compiled" << endl;
			return 1;
#endif
		} else {
			cerr << "ERROR: Invalid audio backend" << endl;
			return 1;
		}

		em->set_audiosource(as);
		em->set_encoder(encoder_ptr(new encoder_af1200stj()));

		as->loop_async();
		em->start_and_run();

		string dest = "APCSU1";
		
		vector<string> repeaters;
		repeaters.push_back("WIDE1-1");
		repeaters.push_back("WIDE2-1");

		ostringstream infostream;
		infostream << "!" 
			   << setfill('0') << fixed 
			   << cfg->lat() << '\\' << cfg->lng() << 'O'
			   << setw(3) << cfg->course() << '/' << setw(3) << cfg->speed()
			   << "/A=" << setw(6) << cfg->alt()
			   << "CSU Rocket Team Test";

		string info = infostream.str();

		cout << info << endl;

		string data = ax25_utils::encode_frame_data_repeater_mode(cfg->callsign(), dest, &repeaters, info);
		
		frame_ptr new_ax25_frame(new frame(reinterpret_cast<const unsigned char*>(data.c_str()), data.length()));
		em->output_packet_to_sc(new_ax25_frame);

		usleep(4000000);
		if (cfg->debug()) {
			cout << "Finished!" << endl;
		}
	} catch (std::exception& e) {
		cerr << "exception: " << e.what() << endl;
	}

	return 0;
}


