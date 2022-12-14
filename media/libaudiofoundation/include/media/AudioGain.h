/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <android/media/AudioGainSys.h>
#include <media/AidlConversion.h>
#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <system/audio.h>

namespace android {

class AudioGain: public RefBase
{
public:
    AudioGain(int index, bool isInput);
    virtual ~AudioGain() = default;

    void setMode(audio_gain_mode_t mode) { mGain.mode = mode; }
    const audio_gain_mode_t &getMode() const { return mGain.mode; }

    void setChannelMask(audio_channel_mask_t mask) { mGain.channel_mask = mask; }
    const audio_channel_mask_t &getChannelMask() const { return mGain.channel_mask; }

    void setMinValueInMb(int minValue) { mGain.min_value = minValue; }
    int getMinValueInMb() const { return mGain.min_value; }

    void setMaxValueInMb(int maxValue) { mGain.max_value = maxValue; }
    int getMaxValueInMb() const { return mGain.max_value; }

    void setDefaultValueInMb(int defaultValue) { mGain.default_value = defaultValue; }
    int getDefaultValueInMb() const { return mGain.default_value; }

    void setStepValueInMb(uint32_t stepValue) { mGain.step_value = stepValue; }
    int getStepValueInMb() const { return mGain.step_value; }

    void setMinRampInMs(uint32_t minRamp) { mGain.min_ramp_ms = minRamp; }
    int getMinRampInMs() const { return mGain.min_ramp_ms; }

    void setMaxRampInMs(uint32_t maxRamp) { mGain.max_ramp_ms = maxRamp; }
    int getMaxRampInMs() const { return mGain.max_ramp_ms; }

    // TODO: remove dump from here (split serialization)
    void dump(std::string *dst, int spaces, int index) const;

    void getDefaultConfig(struct audio_gain_config *config);
    status_t checkConfig(const struct audio_gain_config *config);

    void setUseForVolume(bool canUseForVolume) { mUseForVolume = canUseForVolume; }
    bool canUseForVolume() const { return mUseForVolume; }

    const struct audio_gain &getGain() const { return mGain; }

    bool equals(const sp<AudioGain>& other) const;

    using Aidl = std::pair<media::audio::common::AudioGain, media::AudioGainSys>;
    ConversionResult<Aidl> toParcelable() const;
    static ConversionResult<sp<AudioGain>> fromParcelable(const Aidl& aidl);

private:
    int               mIndex;
    bool              mIsInput;
    struct audio_gain mGain = {};
    bool              mUseForVolume = false;
};

// Conversion routines, according to AidlConversion.h conventions.
ConversionResult<sp<AudioGain>>
aidl2legacy_AudioGain(const AudioGain::Aidl& aidl);
ConversionResult<AudioGain::Aidl>
legacy2aidl_AudioGain(const sp<AudioGain>& legacy);

class AudioGains : public std::vector<sp<AudioGain>>
{
public:
    bool canUseForVolume() const
    {
        for (const auto &gain: *this) {
            if (gain->canUseForVolume()) {
                return true;
            }
        }
        return false;
    }

    int32_t add(const sp<AudioGain>& gain)
    {
        push_back(gain);
        return 0;
    }

    bool equals(const AudioGains& other) const;

    using Aidl = std::pair<
            std::vector<media::audio::common::AudioGain>,
            std::vector<media::AudioGainSys>>;
};

// Conversion routines, according to AidlConversion.h conventions.
ConversionResult<AudioGains>
aidl2legacy_AudioGains(const AudioGains::Aidl& aidl);
ConversionResult<AudioGains::Aidl>
legacy2aidl_AudioGains(const AudioGains& legacy);

} // namespace android
