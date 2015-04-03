#include "main.h"

#ifdef MODPLAYER_ENABLED
#include "modplayer.h"

struct t_modplayerInstance modplayerInstance;

void modplayerGetSampleName(char* buf, uint8_t id) {
	uint16_t offset = OFFSET_SAMPLE_HEADER + SAMPLE_HEADER_LENGTH * id;
	uint8_t i;
	for(i=0; i<22; i++) {
		buf[i] = modplayerInstance.moduleData[offset+i];
	}
}

void modplayerGetSongName(char* buf) {
	uint8_t i;
	for(i=0;i<20;i++) {
		buf[i] = (char)modplayerInstance.moduleData[i];
	}
}

/**
 * reset channel state
 */
void modplayerResetChannelState(struct t_modplayerChannelState *modplayerChannelState) {
	modplayerChannelState->arptableindex[0] = 0;
	modplayerChannelState->arptableindex[1] = 0;
	modplayerChannelState->arptableindex[2] = 0;
	modplayerChannelState->volume_slide = 0;
	modplayerChannelState->pitch_slide = 0;
	modplayerChannelState->vibrato_amplitude = 0;
	modplayerChannelState->vibrato_frequency = 0;
	modplayerChannelState->vibrato_waveform = 0;
	modplayerChannelState->vibrato_phase = 0;
	modplayerChannelState->pitch_slide_limit_lo = 113;
	modplayerChannelState->pitch_slide_limit_hi = 856;
}

/**
 * Initialize data structures
 */
void modplayerInit() {
	uint8_t i;
	for(i=0;i<MOD_CHANNELS;i++) modplayerInstance.channelstate[i].vu = 0;
	for(i=0;i<31;i++) modplayerInstance.sampleheader[i].vu = 0;
	modplayerInstance.samples_rendered = 0;
	modplayerInstance.samples_rendered_tick = 0;
	modplayerInstance.speed_bpm = 111;
	paulaInit(&(modplayerInstance.paulaInstance));
}

/**
 * get highest pattern number
 */
uint8_t modplayerGetHighestPatternNumber() {
	uint8_t max = 0;
	uint32_t offset;
	offset = modplayerInstance.songinfo.offset_songheader + 2;

	uint8_t i;
	for(i=0;i<128;i++) {
		uint8_t tmp = modplayerInstance.moduleData[offset + i];
		if (tmp > max) max = tmp;
	}

	return max;
}

/**
 * copy sample metainfo into RAM
 */
void modplayerCopySampleinfo() {

	// look for M.K.
	if((modplayerInstance.moduleData[OFFSET_MK] == 'M') &&
	   (modplayerInstance.moduleData[OFFSET_MK+1] == '.') &&
	   (modplayerInstance.moduleData[OFFSET_MK+2] == 'K') &&
	   (modplayerInstance.moduleData[OFFSET_MK+3] == '.')) {
		modplayerInstance.songinfo.numsamples = 31;
	} else {
		modplayerInstance.songinfo.numsamples = 15;
	}

	modplayerInstance.songinfo.numchannels = 4;

	//TODO: remove?
	if (modplayerInstance.songinfo.numchannels > MOD_CHANNELS) {
		while(1);
	}

	uint32_t soff=0;
	uint8_t c;

	// read SAMPLEINFO from FLASH into RAM
	for(c = 0; c < modplayerInstance.songinfo.numsamples; c++) {

		modplayerInstance.sampleheader[c].data_offset = soff + 4;
		uint32_t offset = OFFSET_SAMPLE_HEADER + (SAMPLE_HEADER_LENGTH * c);

		modplayerInstance.sampleheader[c].length = (uint8_t)modplayerInstance.moduleData[offset + 22] << 9;
		modplayerInstance.sampleheader[c].length += (uint8_t)modplayerInstance.moduleData[offset + 23] << 1;

		modplayerInstance.sampleheader[c].finetune = (int8_t)modplayerInstance.moduleData[offset + 24];
		modplayerInstance.sampleheader[c].volume = (uint8_t)modplayerInstance.moduleData[offset + 25];

		modplayerInstance.sampleheader[c].loop_start = (uint8_t)modplayerInstance.moduleData[offset + 26] << 9;
		modplayerInstance.sampleheader[c].loop_start += (uint8_t)modplayerInstance.moduleData[offset + 27] << 1;

		modplayerInstance.sampleheader[c].loop_length = (uint8_t)modplayerInstance.moduleData[offset + 28] << 9;
		modplayerInstance.sampleheader[c].loop_length += (uint8_t)modplayerInstance.moduleData[offset + 29] << 1;

		modplayerInstance.sampleheader[c].loop_end = modplayerInstance.sampleheader[c].loop_start + modplayerInstance.sampleheader[c].loop_length;
		modplayerInstance.sampleheader[c].loop = (modplayerInstance.sampleheader[c].loop_length > 2);

		soff += modplayerInstance.sampleheader[c].length;
	}
}

/**
 * copy song metainfo into RAM
 */
void modplayerCopySonginfo() {

	modplayerInstance.songinfo.offset_songheader = OFFSET_SAMPLE_HEADER + SAMPLE_HEADER_LENGTH * modplayerInstance.songinfo.numsamples;

	modplayerInstance.songinfo.offset_patterns = modplayerInstance.songinfo.offset_songheader + 1 + 1 + 128;

	if (modplayerInstance.songinfo.numsamples == 31) modplayerInstance.songinfo.offset_patterns += 4; // Skip Idenitifier M.K.

	uint8_t i;

	modplayerInstance.songinfo.numpatterns = modplayerGetHighestPatternNumber(modplayerInstance) + 1;
	modplayerInstance.songinfo.numsongpos = modplayerInstance.moduleData[modplayerInstance.songinfo.offset_songheader];

	for(i=0;i<128;i++) {
		modplayerInstance.songinfo.patterntable[i] = modplayerInstance.moduleData[modplayerInstance.songinfo.offset_songheader + 2 + i];
	}
}


/**
 * Load .MOD from flash and copy metadata to RAM
 */
void modplayerLoadModule(const char* moddata, const uint32_t size) {

	modplayerInstance.moduleData = moddata;
	modplayerInstance.moduleSize = size;

	modplayerCopySampleinfo();
	modplayerCopySonginfo();

	// set data offset for sampledata
	modplayerInstance.songinfo.offset_samples = modplayerInstance.songinfo.offset_patterns + ((uint32_t)modplayerInstance.songinfo.numpatterns * 1024);

	modplayerInstance.songpos = 0;
	modplayerInstance.activerow = 0;
	modplayerInstance.tick = 0;
	modplayerInstance.speed = 6;
	modplayerInstance.arpindex = 0;
}

/**
 *
 */
void modplayerResetFx(struct t_modplayerChannelState *channelState) {
	channelState->arptableindex[1] = 0;
	channelState->arptableindex[2] = 0;
	channelState->volume_slide = 0;
	channelState->pitch_slide = 0;
	channelState->vibrato_amplitude = 0;
}

/**
 * trigger sample on channel
 */
void modplayerTriggerOffset(uint8_t ch, uint8_t sample_id, uint16_t rate, uint16_t offset) {

	struct t_paulaChannel *paulaChannel = &(modplayerInstance.paulaInstance.paulachannel[ch]);
	struct t_modplayerChannelState *channelState = &(modplayerInstance.channelstate[ch]);

	// visualisation buffers
	channelState->vu = 100.0f;
	modplayerInstance.sampleheader[sample_id].vu = 100.0f;

	// reset channel state
	modplayerResetChannelState(channelState);

	if (rate > 0) {
		float factor = finetune[modplayerInstance.sampleheader[sample_id].finetune & 0x0f];
		channelState->triggered_rate = (float)rate * factor;
	}

	if (sample_id != 255) {
		channelState->sample_id = sample_id;
	}

	paulaChannel->addr = modplayerInstance.songinfo.offset_samples + modplayerInstance.sampleheader[channelState->sample_id].data_offset;
	paulaChannel->length = modplayerInstance.sampleheader[channelState->sample_id].length;

	channelState->volume = modplayerInstance.sampleheader[channelState->sample_id].volume;

	paulaChannel->loop_start = modplayerInstance.sampleheader[channelState->sample_id].loop_start;
	paulaChannel->loop_end = modplayerInstance.sampleheader[channelState->sample_id].loop_end;
	paulaChannel->loop_length = paulaChannel->loop_end - paulaChannel->loop_start;
	paulaChannel->loop_enable = modplayerInstance.sampleheader[channelState->sample_id].loop;

	paulaChannel->position = offset;
	paulaChannel->playing = 1;

	channelState->rate_before_fx = channelState->triggered_rate;

	paulaChannel->finalrate = channelState->triggered_rate;
	paulaChannel->finalrate_reciproc = 1.0f / paulaChannel->finalrate;
}

/**
 * trigger sample on channel with offset
 */
void modplayerTrigger(uint8_t ch, uint8_t sample_id, uint16_t rate) {
	modplayerTriggerOffset(ch, sample_id, rate, 0);
}

/**
 * untrigger sample on channel
 */
void modplayerUntrigger(uint8_t ch) {

	struct t_paulaChannel *paulaChannel = &(modplayerInstance.paulaInstance.paulachannel[ch]);
	struct t_modplayerChannelState *channelState = &(modplayerInstance.channelstate[ch]);

	// reset channel state
	modplayerResetChannelState(channelState);

	paulaChannel->position = 0;
	paulaChannel->playing = 0;

}

/**
 *
 */
void modplayerProcessRow() {

	//TODO: do this incremental rather than explicit - possible?
	uint32_t pre_offset = modplayerInstance.songinfo.offset_patterns;
	pre_offset += (modplayerInstance.songinfo.patterntable[modplayerInstance.songpos] << 10);
	pre_offset += (modplayerInstance.activerow << 4);

	uint8_t ch;
	for (ch = 0; ch < modplayerInstance.songinfo.numchannels; ch++) {

		struct t_modplayerChannelState *channelState = &(modplayerInstance.channelstate[ch]);

		// Decode one division
		uint32_t offset = pre_offset + (ch << 2);

		//TODO: read 4 bytes at once; we have a 32bit architecture - possible/makes sense?
		uint8_t byte1 = modplayerInstance.moduleData[offset];
		uint8_t byte2 = modplayerInstance.moduleData[offset+1];
		uint8_t byte3 = modplayerInstance.moduleData[offset+2];
		uint8_t byte4 = modplayerInstance.moduleData[offset+3];

		uint16_t rate = ((byte1 & 0x0f) << 8) + byte2;
		uint8_t sample_id = (byte1 & 0xf0) + (byte3 >> 4);
		uint8_t effectdata = byte4;
		uint8_t effectid = byte3 & 0x0f;
		uint8_t effect_x = effectdata >> 4;
		uint8_t effect_y = effectdata & 0xf;

		// trigger note if necessary
		if ((sample_id > 0) || (rate > 0)) {
			// 0x3 = slide to note, no retrigger
			if ((effectid != 0x3) && (effectid != 0x09)) {
				modplayerTrigger(ch,sample_id - 1,rate & 0x0fff);
			}
		}

		// main effect switch
		switch(effectid) {

			case 0x0: // arpeggio
				modplayerResetFx(channelState);
				channelState->arptableindex[1] = effect_x;
				channelState->arptableindex[2] = effect_y;
				break;

			case 0x1: // pitch slide
				modplayerResetFx(channelState);
				channelState->pitch_slide = -effectdata;
				break;

			case 0x2: // pitch slide
				modplayerResetFx(channelState);
				channelState->pitch_slide = effectdata;
				break;

			case 0x3: // pitch slide
				// TODO: reset other effects(?)
				if ((effect_y != 0) || (effect_x != 0)) {
					if (rate > 0) {
						if (channelState->rate_before_fx > rate) {
							// slide up, rate down
							channelState->pitch_slide = -effectdata;
							channelState->pitch_slide_limit_lo = rate;
						} else {
							// slide down, rate up
							channelState->pitch_slide = effectdata;
							channelState->pitch_slide_limit_hi = rate;
						}
					}
				}
				break;

			case 0x4: // vibrator
				// TODO: reset other effects (?)
				if ((effect_y != 0) || (effect_x != 0)) {
					channelState->vibrato_amplitude = effect_y;
					channelState->vibrato_frequency = effect_x;
					channelState->vibrato_phase = 0;
				}
				break;

			case 0x6: // Continue 'Vibrato', but also do Volume slide
				// Reset FX but Vibrator
				channelState->arptableindex[1] = 0;
				channelState->arptableindex[2] = 0;
				channelState->volume_slide = 0;
				channelState->pitch_slide = 0;

				if (effect_x > 0) {
					channelState->volume_slide = effect_x;
				} else if (effect_y > 0) {
					channelState->volume_slide = -effect_y;
				}
				break;

			case 0x7: // tremolo
				// TODO: implement tremolo
				break;

			//case 0x8: // unused

			case 0x9: // sample offset
				modplayerTriggerOffset(ch,sample_id - 1,rate & 0x0fff, effect_x * 4096 + effect_y * 256);
				break;

			case 0xa: // volume slide
				modplayerResetFx(channelState);
				if (effect_x > 0) {
					channelState->volume_slide = effect_x;
				} else if (effect_y > 0) {
					channelState->volume_slide = -effect_y;
				}
				break;

			case 0xb: // jump to pattern
				modplayerResetFx(channelState);
				modplayerInstance.songpos = effectdata;
				modplayerInstance.activerow = 0;
				break;

			case 0xc: // set volume
				modplayerResetFx(channelState);
				channelState->volume = effectdata;
				//pch->volume = effectdata;
				break;

			case 0xd: // end current pattern
				modplayerResetFx(channelState);
				modplayerInstance.songpos++;
				modplayerInstance.activerow = 0;
				break;

			case 0xe: // special effects

				switch(effect_y) {

					//case 0x0: // filter
						// NOT SUPPORTED
					//	break;

					case 0x1: // fineslide up
						// TODO: implement
						break;

					case 0x2: // fineslide down
						// TODO: implement
						break;

					case 0x3: // Set glissando on/off
						// TODO: implement
						break;

					case 0x4: // Set vibrato waveform
						// TODO: implement
						break;

					case 0x5: // Set finetune value
						// TODO: implement
						break;

					case 0x6: // Loop pattern
						// TODO: implement
						break;

					case 0x7: //Set tremolo waveform
						// TODO: implement
						break;

					// case 0x8: unused

					case 0x9: // Retrigger sample
						// TODO: implement
						break;

					case 0xa: // Fine volume slide up
						// TODO: implement
						break;

					case 0xb: // Fine volume slide down
						// TODO: implement
						break;

					case 0xc: // Cut sample
						modplayerUntrigger(ch);
						break;

					//case 0xd: //  Delay sample
						// NOT SUPPORTED
					//	break;

					//case 0xe: // Delay pattern
						// NOT SUPPORTED
					//	break;

					//case 0xf: // Invert loop
						// NOT SUPPORTED
					//	break;

				}

				break;

			case 0xf: // set speed
				modplayerResetFx(channelState);
				if (effectdata <= 32) {
					// set ticks per row
					modplayerInstance.speed = effectdata;
				} else {
					// set BPM
				}
				break;

			default:
				break;
		}
	}
}

/**
 * advance to next row
 */
void modplayerNextRow() {
	modplayerInstance.activerow++;
	if (modplayerInstance.activerow > 63) {
		modplayerInstance.activerow = 0;
		modplayerInstance.songpos++;
		if(modplayerInstance.songpos >= modplayerInstance.songinfo.numsongpos) modplayerInstance.songpos = 0;
	}
}

/**
 * process one frame
 * one row consists of one or more "frames" (speed=frames, i.e. speed=6, 6 frames per row)
 */
void modplayerProcessFrame() {

	uint8_t ch;
	for (ch=0 ; ch < modplayerInstance.songinfo.numchannels; ch++) {

		struct t_paulaChannel *paulaChannel = &(modplayerInstance.paulaInstance.paulachannel[ch]);
		struct t_modplayerChannelState *channelState = &(modplayerInstance.channelstate[ch]);

		if (paulaChannel->playing) {

			// arpeggio
			paulaChannel->finalrate = (uint16_t)channelState->rate_before_fx * arptable_reciproc[channelState->arptableindex[modplayerInstance.arpindex]];

			if (channelState->vibrato_amplitude > 0) {
				// vibrator
				channelState->vibrato_phase += channelState->vibrato_frequency;
				if (channelState->vibrato_phase > 63) channelState->vibrato_phase -= 64;
				paulaChannel->finalrate *= vibratotable[channelState->vibrato_amplitude][channelState->vibrato_phase];
			}

			paulaChannel->finalrate_reciproc = 1.0f / paulaChannel->finalrate;

			if (modplayerInstance.tick != 0) {
				// pitch slide
				if (channelState->pitch_slide != 0) {
					channelState->rate_before_fx += channelState->pitch_slide;

					// limit pitch slide to limits
					if (channelState->rate_before_fx > channelState->pitch_slide_limit_hi) {
						channelState->rate_before_fx = channelState->pitch_slide_limit_hi;
					} else if (channelState->rate_before_fx < channelState->pitch_slide_limit_lo) {
						channelState->rate_before_fx = channelState->pitch_slide_limit_lo;
					}
				}

				// volume slide
				if (channelState->volume_slide != 0) {
					channelState->volume += channelState->volume_slide;
					if (channelState->volume < 0) channelState->volume = 0;
					else if (channelState->volume > 64) channelState->volume = 64;
				}

			}

			paulaChannel->volume = channelState->volume;
			// decrease channel "percents" for display
			modplayerInstance.channelstate[ch].vu *= 0.98;
		} else {
			// decrease channel "percents" for display faster thus channels is inactive
			modplayerInstance.channelstate[ch].vu *= 0.93;
		}

	}

	// decrease sample "percents" for display
	for(ch=0; ch<modplayerInstance.songinfo.numsamples; ch++) {
		modplayerInstance.sampleheader[ch].vu *= 0.97;
	}

	modplayerInstance.arpindex++;
	if (modplayerInstance.arpindex > 2) modplayerInstance.arpindex = 0;
}

/**
 *
 */
void modplayerTick(void) {

	if (modplayerInstance.tick == 0) {
		modplayerProcessRow();
		modplayerNextRow();
	}

	modplayerProcessFrame();

	modplayerInstance.tick++;
	if (modplayerInstance.tick >= modplayerInstance.speed) {
		modplayerInstance.tick = 0;
	}
}

#endif
