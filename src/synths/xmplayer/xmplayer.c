#include "main.h"
#ifdef XMPLAYER_ENABLED

#include "../../testfile.h"

#include "xmplayer.h"

struct t_xmplayerInstance xmplayerInstance;


/**
 * reset channel state
 */
void xmplayerResetChannelState(struct t_xmplayerChannelState *xmplayerChannelState) {
	xmplayerChannelState->arptableindex[0] = 0;
	xmplayerChannelState->arptableindex[1] = 0;
	xmplayerChannelState->arptableindex[2] = 0;
	xmplayerChannelState->volume_slide = 0;
	xmplayerChannelState->pitch_slide = 0;
	xmplayerChannelState->vibrato_amplitude = 0;
	xmplayerChannelState->vibrato_frequency = 0;
	xmplayerChannelState->vibrato_waveform = 0;
	xmplayerChannelState->vibrato_phase = 0;
	xmplayerChannelState->pitch_slide_limit_lo = 113;
	xmplayerChannelState->pitch_slide_limit_hi = 856;
}

/**
 * Initialize data structures
 */
void xmplayerInit() {
	uint8_t i;
	xmplayerInstance.samples_rendered = 0;
	xmplayerInstance.samples_rendered_tick = 0;
	xmPaulaInit(&(xmplayerInstance.xmPaulaInstance));
}

/**
 *
 */
void xmplayerLoadModule() {
	xmplayerInstance.songpos = 0;
	xmplayerInstance.activerow = 0;
	xmplayerInstance.tick = 0;
	xmplayerInstance.speed = 6;
	xmplayerInstance.arpindex = 0;
}

/**
 *
 */
void xmplayerResetFx(struct t_xmplayerChannelState *channelState) {
	channelState->arptableindex[1] = 0;
	channelState->arptableindex[2] = 0;
	channelState->volume_slide = 0;
	channelState->pitch_slide = 0;
	channelState->vibrato_amplitude = 0;
}

/**
 * Untrigger channel (note off)
 */
void xmplayerUntrigger(uint8_t ch) {
	// reset channel state
	xmplayerResetChannelState(channelstate);
	channelstate->triggered_note = 0;
}


/**
 * trigger sample on channel
 */
void xmplayerTrigger(uint8_t ch, uint8_t instrument, uint16_t note) {

	struct t_xmplayerChannelState *channelstate = &(xmplayerInstance.channelstate[ch]);

	// reset channel state
	xmplayerResetChannelState(channelstate);

	channelstate->triggered_note = note;
	channelstate->instrument = instrument;

	/*
	xmPaulaChannel->addr = xmplayerInstance.songinfo.offset_samples + xmplayerInstance.sampleheader[channelState->sample_id].data_offset;
	xmPaulaChannel->length = xmplayerInstance.sampleheader[channelState->sample_id].length;

	channelState->volume = xmplayerInstance.sampleheader[channelState->sample_id].volume;

	xmPaulaChannel->loop_start = xmplayerInstance.sampleheader[channelState->sample_id].loop_start;
	xmPaulaChannel->loop_end = xmplayerInstance.sampleheader[channelState->sample_id].loop_end;
	xmPaulaChannel->loop_length = xmPaulaChannel->loop_end - xmPaulaChannel->loop_start;
	xmPaulaChannel->loop_enable = xmplayerInstance.sampleheader[channelState->sample_id].loop;

	xmPaulaChannel->position = 0;
	xmPaulaChannel->playing = 1;

	channelState->rate_before_fx = channelState->triggered_rate;

	xmPaulaChannel->finalrate = channelState->triggered_rate;
	xmPaulaChannel->finalrate_reciproc = 1.0f / xmPaulaChannel->finalrate;
	*/
}

/**
 *
 */
void xmplayerProcessRow() {

	uint8_t activePatternId = xmplayerSongInfo.patternOrderTable[xmplayerInstance.songpos];
	struct t_xmplayerPatternHeader *activePatternHeader = &xmplayerPatternHeader[activePatternId];
	uint8_t *patternData = activePatternHeader->patternData;

	// Move pointer to the first channel of the active row
	patternData += xmplayerInstance.activerow * 5 * xmplayerSongInfo.numChannels;

	uint8_t ch;
	for (ch = 0; ch < xmplayerSongInfo.numChannels; ch++) {

		uint8_t note = *patternData++;
		uint8_t instrument = *patternData++;
		uint8_t volumeCol = *patternData++;
		uint8_t effectType = *patternData++;
		uint8_t effectParam = *patternData++;

		// patternData pointer points now to the next channel

		// trigger or untrigger note if necessary
		if (note == 97) {
			xmplayerUntrigger(ch);
		} else if (note > 0) {
			// 0x3 = slide to note, no retrigger
			//if (effectid != 0x3) {
				xmplayerTrigger(ch, instrument, note);
			//}
		}

		/*
		// main effect switch
		switch(effectid) {

			case 0x0: // arpeggio
				xmplayerResetFx(channelState);
				channelState->arptableindex[1] = effect_x;
				channelState->arptableindex[2] = effect_y;
				break;

			case 0x1: // pitch slide
				xmplayerResetFx(channelState);
				channelState->pitch_slide = -effectdata;
				break;

			case 0x2: // pitch slide
				xmplayerResetFx(channelState);
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
				// TODO: implement sample offset
				break;

			case 0xa: // volume slide
				xmplayerResetFx(channelState);
				if (effect_x > 0) {
					channelState->volume_slide = effect_x;
				} else if (effect_y > 0) {
					channelState->volume_slide = -effect_y;
				}
				break;

			case 0xb: // jump to pattern
				xmplayerResetFx(channelState);
				xmplayerInstance.songpos = effectdata;
				xmplayerInstance.activerow = 0;
				break;

			case 0xc: // set volume
				xmplayerResetFx(channelState);
				channelState->volume = effectdata;
				//pch->volume = effectdata;
				break;

			case 0xd: // end current pattern
				xmplayerResetFx(channelState);
				xmplayerInstance.songpos++;
				xmplayerInstance.activerow = 0;
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

					//case 0xc: // Cut sample
					//	// NOT SUPPORTED
					//	break;

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
				xmplayerResetFx(channelState);
				if (effectdata <= 32) {
					xmplayerInstance.speed = effectdata;
				}
				break;

			default:
				break;
		}
		*/
	}

}

/**
 * advance to next row
 */
void xmplayerNextRow() {
	/*
	xmplayerInstance.activerow++;
	if (xmplayerInstance.activerow > 63) {
		xmplayerInstance.activerow = 0;
		xmplayerInstance.songpos++;
		if(xmplayerInstance.songpos >= xmplayerInstance.songinfo.numsongpos) xmplayerInstance.songpos = 0;
	}
	*/
}

/**
 * process one frame
 * one row consists of one or more "frames" (speed=frames, i.e. speed=6, 6 frames per row)
 */
void xmplayerProcessFrame() {

	/*
	uint8_t ch;
	for (ch=0 ; ch < xmplayerInstance.songinfo.numchannels; ch++) {

		struct t_xmPaulaChannel *xmPaulaChannel = &(xmplayerInstance.xmPaulaInstance.xmPaulachannel[ch]);
		struct t_xmplayerChannelState *channelState = &(xmplayerInstance.channelstate[ch]);

		if (xmPaulaChannel->playing) {

			// arpeggio
			//xmPaulaChannel->finalrate = (uint16_t)channelState->rate_before_fx * arptable_reciproc[channelState->arptableindex[xmplayerInstance.arpindex]];

			if (channelState->vibrato_amplitude > 0) {
				// vibrator
				channelState->vibrato_phase += channelState->vibrato_frequency;
				if (channelState->vibrato_phase > 63) channelState->vibrato_phase -= 64;
				//xmPaulaChannel->finalrate *= vibratotable[channelState->vibrato_amplitude][channelState->vibrato_phase];
			}

			xmPaulaChannel->finalrate_reciproc = 1.0f / xmPaulaChannel->finalrate;

			if (xmplayerInstance.tick != 0) {
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

			xmPaulaChannel->volume = channelState->volume;
			// decrease channel "percents" for display
			xmplayerInstance.channelstate[ch].vu *= 0.98;
		} else {
			// decrease channel "percents" for display faster thus channels is inactive
			xmplayerInstance.channelstate[ch].vu *= 0.93;
		}

	}

	// decrease sample "percents" for display
	for(ch=0; ch<xmplayerInstance.songinfo.numsamples; ch++) {
		xmplayerInstance.sampleheader[ch].vu *= 0.97;
	}

	xmplayerInstance.arpindex++;
	if (xmplayerInstance.arpindex > 2) xmplayerInstance.arpindex = 0;
*/
}

/**
 *
 */
void xmplayerTick(void) {

	if (xmplayerInstance.tick == 0) {
		xmplayerProcessRow();
		xmplayerNextRow();
	}

	xmplayerProcessFrame();

	xmplayerInstance.tick++;
	if (xmplayerInstance.tick >= xmplayerInstance.speed) {
		xmplayerInstance.tick = 0;
	}
}

#endif
