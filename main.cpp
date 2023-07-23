#include <iostream>
#include <cstring>
#include <mosquittopp.h> // lib_init, lib_cleanup
#include <atomic>
#include <condition_variable>
#include <chrono>

#include "MqttDataClient.hpp"

std::atomic<bool> running(false);
std::condition_variable cv;
std::mutex cv_m;

using namespace std::chrono_literals;

enum CMD{
    invalid = 0,
    quit = 1,
    read_sensor = 2,
    reset_sensor =3
};

CMD cmd = invalid;
std::atomic<CMD> atomic_cmd(invalid);

void mqtt_sub_atomic_boolean(MqttDataClient& mqttDataClient)
{
    mqttDataClient.loop_start();
    running = true;

    while (running){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    mqttDataClient.disconnect();
    mqttDataClient.loop_stop(false);
}

void sensor_svr(MqttDataClient& mqttDataClient)
{
    mqttDataClient.loop_start();
    std::unique_lock<std::mutex> ul(cv_m);  //lock is applied on mutex m by thread t1
    while(true){

        cv.wait(ul,[] {return atomic_cmd != invalid;});  //waits until the condition becomes true

        if (atomic_cmd == quit){
            break;
        }
        else if (atomic_cmd == read_sensor){
            std::cout << "read sensor \n";
            //clear cmd
            atomic_cmd = invalid;
        }
        else if (atomic_cmd == reset_sensor){
            std::cout << "reset sensor \n";
            //clear cmd
            atomic_cmd = invalid;
        }


    }
     

    mqttDataClient.disconnect();
    mqttDataClient.loop_stop(false);
}





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

        if (std::strcmp( static_cast<const char*> (msg->payload), "quit") == 0){
            std::cout << "command received: quit\n" ;
            atomic_cmd = quit;
            cv.notify_all();
        }
        else if (std::strcmp( static_cast<const char*> (msg->payload), "read_sensor") == 0){
            std::cout << "command received: read_sensor\n" ;
            atomic_cmd = read_sensor;
            cv.notify_all();
        }
        else if (std::strcmp( static_cast<const char*> (msg->payload), "reset_sensor") == 0){
            std::cout << "command received: read_sensor\n" ;
            atomic_cmd = reset_sensor;
            cv.notify_all();
        }
        
    };

    mqttDataClient.register_on_message("topic1", topic1_cb);
    mqttDataClient.register_on_message("topic2", topic2_cb);
    mqttDataClient.register_on_message("topic_command", topic_command_cb);


    std::thread t1(sensor_svr, std::ref(mqttDataClient));

    t1.join();






    return 0;
}