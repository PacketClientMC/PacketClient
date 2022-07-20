#include "IRCCommand.h"

using namespace std;
IRCCommand::IRCCommand() : IMCCommand("irc", "Talk to other people on Packet Client", "<message>") {
}

bool IRCCommand::execute(vector<string>* args) {
    assertTrue(g_Data.getLocalPlayer() != nullptr);
    string option = args->at(1); transform(option.begin(), option.end(), option.begin(), ::tolower);
    auto player = g_Data.getLocalPlayer();
    
    if (args->size() > 1) {
        ostringstream os;
        for (int i = 0; i < args->size(); i++) {
            if (i > 1) os << " ";
            os << args->at(i);
        }
        string text = os.str().substr(3);
        string name = player->getNameTag()->getText(); name = Utils::sanitize(name); name = name.substr(0, name.find('\n'));

        string test = "<" + name + "> " + text;
        IRC irc; irc.sendMessage(test);
    }
}