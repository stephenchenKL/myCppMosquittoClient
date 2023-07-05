#include <iostream>
#include <mosquittopp.h> // lib_init, lib_cleanup

#include "MqttDataClient.hpp"



int main(int argc, char *argv[])
{   
    // Initialise the data file reading/writing library.
    mosqpp::lib_init();

    MqttDataClient          mqttDataClient;

    mqttDataClient.connect("localhost", 1883, 60);


    auto consume_anytopic_callback = [](const mosquitto_message* msg){
        if(!msg->payload) { return; }
        std::cout << (msg->payload);
    };
    auto consume_autoGain_callback = [](void){
        std::cout << "Auto Gain!";
    };

    mqttDataClient.subscribe(nullptr, "command", 2);
    mqttDataClient.register_on_message("autoGain", consume_anytopic_callback);
    mqttDataClient.register_on_command("scan", consume_autoGain_callback);
    mqttDataClient.loop_forever();



    return 0;
}