#include <iostream>
#include <cstring>
#include <mosquittopp.h> // lib_init, lib_cleanup

#include "MqttDataClient.hpp"




int main(int argc, char *argv[])


{   
    // Initialise the data file reading/writing library.
    mosqpp::lib_init();

    MqttDataClient          mqttDataClient;

    mqttDataClient.connect("localhost", 1883, 60);

    mqttDataClient.subscribe(nullptr, "topic1", 2);
    mqttDataClient.subscribe(nullptr, "topic2", 2);
    mqttDataClient.subscribe(nullptr, "topic_command", 2);
    

    auto topic1_cb = [](const mosquitto_message* msg){
        std::cout << "in topic1_cb \n";

        if(!msg->payload) { return; }
        
        std::cout << msg->mid << std::endl;
        std::cout << msg->topic << std::endl;
        const char* payload = static_cast<const char*> (msg->payload);
        std::cout << payload << std::endl;
        std::cout << msg->payloadlen << std::endl;
        std::cout << msg->qos << std::endl;
        std::cout << msg->retain << std::endl;
        
    };

    auto topic2_cb = [](const mosquitto_message* msg){
        std::cout << "in topic2_cb \n";

        if(!msg->payload) { return; }
        
        std::cout << msg->mid << std::endl;
        std::cout << msg->topic << std::endl;
        std::cout << static_cast<const char*> (msg->payload) << std::endl;
        std::cout << msg->payloadlen << std::endl;
        std::cout << msg->qos << std::endl;
        std::cout << msg->retain << std::endl;
        
    };

    auto topic_command_cb = [](const mosquitto_message* msg){
        std::cout << "in topic_command_cb \n";

        if(!msg->payload) { return; }
        
        std::cout << msg->mid << std::endl;
        std::cout << msg->topic << std::endl;
        std::cout << static_cast<const char*> (msg->payload) << std::endl;
        std::cout << msg->payloadlen << std::endl;
        std::cout << msg->qos << std::endl;
        std::cout << msg->retain << std::endl;

        if (std::strcmp( static_cast<const char*> (msg->payload), "stop") == 0){
            std::cout << "Please Stop! \n" ;
        }
        
    };

    mqttDataClient.register_on_message("topic1", topic1_cb);
    mqttDataClient.register_on_message("topic2", topic2_cb);
    mqttDataClient.register_on_message("topic_command", topic_command_cb);


    mqttDataClient.loop_forever();



    return 0;
}