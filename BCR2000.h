/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "SimpleDiscoverableDevice.h"
#include "MidiController.h"
#include "MidiChannel.h"

#include "Logger.h"

#include <vector>
#include <string>

namespace midikraft {

	class BCRdefinition;

	class BCR2000 : public SimpleDiscoverableDevice {
	public:
		BCR2000() {}

		void writeToFile(std::string const &filename, std::string const &bcl) const;
		std::vector<MidiMessage> convertToSyx(std::string const &bcl) const;

		static std::string convertSyxToText(const MidiMessage &message);
		static bool isSysexFromBCR2000(const MidiMessage& message);

		static void sendSysExToBCR(SafeMidiOutput *midiOutput, std::vector<MidiMessage> const &messages, SimpleLogger *logger, std::function<void()> const &whenDone = []() {});

		// Implementation of DiscoverableDevice
		virtual std::string getName() const override;
		virtual MidiMessage deviceDetect(int channel) override;
		virtual int deviceDetectSleepMS() override;
		virtual MidiChannel channelIfValidDeviceResponse(const MidiMessage &message) override;
		virtual bool needsChannelSpecificDetection() override;

		static std::string generateBCRHeader();
		static std::string generateBCREnd(int recallPreset);
		static std::string generatePresetHeader(std::string const &presetName);
		static std::string generateBCRFooter(int storagePlace);
		static std::string generateAllEncoders(std::vector<std::pair<BCRdefinition *, std::string>> &all_entries);

		static std::string syxToBCRString(MidiMessage const &syx);

		// Preset handling in the BCR2000 itself. This is fully automated management, so we do not need a Librarian
		int indexOfPreset(std::string const &name);
		void selectPreset(MidiController *controller, int presetIndex);
		void refreshListOfPresets(std::function<void()> callback);
		void invalidateListOfPresets();

	private:
		uint8 sysexCommand(const MidiMessage &message) const;
		std::vector<uint8> createSysexCommandData(uint8 commandCode) const;
		std::vector<std::string> bcrPresets_; // These are the names of the 32 presets stored in the BCR2000

		struct TransferCounters {
			int numMessages;
			int receivedMessages;
			int lastLine;
			int overflowCounter;
		};
	};

}
