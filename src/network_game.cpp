/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "network_game.h"

using namespace std;

bool Network_Game::receive_game_packet (RakNet::Packet* packet, const RakNet::MessageID& packet_id) {
    /**if(packet_id==ID_GAME_EXAMPLE){
        // Do something with this packet

        return true;
       }*/

    return false;
}

string Network_Game::allow_new_connection () {
    /**if(SOME_EXAMPLE_REASON_TO_REJECT_NEW_CONNECTIONS){
        return "Example reason";
       }*/

    return "";
}

void Network_Game::write_initial_game_data (RakNet::BitStream& bitstream) {
    // bitstream.WriteCompressed();
}

void Network_Game::read_initial_game_data (RakNet::BitStream& bitstream) {
    // bitstream.ReadCompressed();
}

void Network_Game::write_update (RakNet::BitStream& bitstream) {
    // bitstream.WriteCompressed();
}

void Network_Game::read_update (RakNet::BitStream& bitstream) {
    // bitstream.ReadCompressed();
}

void Network_Game::write_server_ready (RakNet::BitStream& bitstream) {
    // Write game commands
}

void Network_Game::read_server_ready (RakNet::BitStream& bitstream) {
    // Read game commands
}

void Network_Game::write_client_ready (RakNet::BitStream& bitstream) {
    // Write desync detection data, etc.
}

void Network_Game::read_client_ready (RakNet::BitStream& bitstream) {
    // Read desync detection data, etc.
}
