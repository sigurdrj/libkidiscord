#include <iostream>
#include "libkidiscord.hpp"

int main(){
	libkidiscord::Client client;

	client.set_auth_token("AUTH TOKEN GOES HERE");

	/* Sending a message */

	//                  Message content           Channel ID
	client.send_message("This is a test message", 123456789012345678);

	/* Grabbing messages from a channel */

	//                                  Channel ID          Max limit of msgs (default = 50)
	auto messages = client.get_messages(123456789012345678, 15);

	/* Show the messages */

	client.print_msgs(messages.first);
}
