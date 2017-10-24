#include <unordered_map>
#include "rack.hpp"
#include "rtmidi/RtMidi.h"


using namespace rack;

struct IgnoreFlags {
	bool midiSysex = true;
	bool midiTime = true;
	bool midiSense = true;
};

/**
 * This class allows to use one instance of rtMidiIn with
 * multiple modules. A MidiIn port will be opened only once while multiple
 * instances can use it simultaniously, each receiving all its incoming messages.
 */

struct MidiInWrapper : RtMidiIn {

	std::unordered_map<int, IgnoreFlags> ignoresMap;

	int uid_c = 0;
	int subscribers = 0;

	MidiInWrapper() : RtMidiIn() {
		idMessagesMap = {};
		idStampsMap = {};
	};

	int add() {
		int id = ++uid_c;
		subscribers++;
		idMessagesMap[id] = {};
		idStampsMap[id] = {};

		return id;
	}

	void erase(int id) {
		subscribers--;
		idMessagesMap.erase(id);
		idStampsMap.erase(id);
		ignoresMap.erase(id);
	}
};

struct MidiIO {
private:
	static std::unordered_map<std::string, MidiInWrapper *> midiInMap;
	/* TODO: add for midi out*/
	int id = -1;
	std::string deviceName = "";
	bool isOut = false;

public:
	int channel;


	MidiIO(bool isOut = false);

	~MidiIO() {
		close();
	}

	std::vector<std::string> getDevices();

	void openDevice(std::string deviceName);

	void setIgnores(bool ignoreSysex = true, bool ignoreTime = true, bool ignoreSense = true);

	std::string getDeviceName();

	void setChannel(int channel);

	double getMessage(std::vector<unsigned char> *msg);

	json_t *addBaseJson(json_t *rootJ);

	void baseFromJson(json_t *rootJ);

	bool isPortOpen();

	void close();

	/* called when midi port is set */
	virtual void resetMidi()=0;

	/* called if a user switches or sets the deivce (and after this device is initialised)*/
	virtual void onDeviceChange(){};
};

//////////////////////
// MIDI module widgets
//////////////////////

struct MidiItem : MenuItem {
	MidiIO *midiModule;

	void onAction();
};

struct MidiChoice : ChoiceButton {
	MidiIO *midiModule;

	void onAction();

	void step();
};

struct ChannelItem : MenuItem {
	MidiIO *midiModule;
	int channel;

	void onAction();
};

struct ChannelChoice : ChoiceButton {
	MidiIO *midiModule;

	void onAction();

	void step();
};


struct MidiToCVWidget : ModuleWidget {
	MidiToCVWidget();

	void step();
};

struct MIDICCToCVWidget : ModuleWidget {
	MIDICCToCVWidget();

	void step();
};

struct MIDIClockToCVWidget : ModuleWidget {
	MIDIClockToCVWidget();

	void step();
};

struct MIDITriggerToCVWidget : ModuleWidget {
	MIDITriggerToCVWidget();

	void step();
};

struct QuadMidiToCVWidget : ModuleWidget {
	QuadMidiToCVWidget();

	void step();
};
