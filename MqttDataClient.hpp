#pragma once

#include <mosquittopp.h>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <functional>


/**
 * @brief MQTT data client to interface with  BLIDataSvc MQTT data server
 */
class MqttDataClient: public mosqpp::mosquittopp {
private:
    typedef std::function<void(const mosquitto_message*)>   MessageCallback_t;
    typedef std::function<void(void)>                       VoidCallback_t;
    
    // map of topic to different hooks
    std::unordered_map<std::string, MessageCallback_t>  table_on_msg;
    std::unordered_map<std::string, VoidCallback_t>     table_on_cmd;

    void on_message(const struct mosquitto_message *message) {
        std::cout << "on_message topic: " << message->topic << std::endl;
        std::string topic {message->topic};
        if(table_on_msg.find(topic) != table_on_msg.end()){
            table_on_msg.at(topic)(message);
        } else {
            std::cout << "no on_message callback for topic " << topic << '\n';
        }
    }

public:
    MqttDataClient(const char *id=nullptr, bool clean_session=true): mosqpp::mosquittopp(id, clean_session) {}
    ~MqttDataClient() = default;
    MqttDataClient(const MqttDataClient&) = delete;
    MqttDataClient &  operator=(const  MqttDataClient &)  = delete;

    /**
     * @brief register callback that will invoked when a command is published by calling pub_command()
     * @param VoidCallback_t std::function<void(void)>
     */
    void register_on_command(std::string cmd, VoidCallback_t cb) { table_on_cmd[cmd] = cb; }

    /**
     * @brief register callback that will invoked upon receiving message from this topic
     * @param MessageCallback_t std::function<void(const mosquitto_message*)>
     */
    void register_on_message(std::string topic, MessageCallback_t cb) 
    { 
        table_on_msg[topic] = cb; 
        std::cout << "register_on_message, topic:" << topic  << std::endl;
    }

    /**
     * @brief Publish a command message and hope a DataServer answers.
     * @param cmd_params vector of strings, where 1st elem is the command, and rest are the parameters
     * @return int zero if successful, and non-zero otherwise
     */
    int  pub_command(std::vector<std::string> cmd_params) {
        if(cmd_params.size() < 1)   { return 1; }
        
        if(table_on_cmd.find(cmd_params[0]) != table_on_cmd.end()){
            table_on_cmd.at(cmd_params[0])();
        }
        
        std::stringstream ss;
        ss << cmd_params[0];
        for(int i = 1; i < cmd_params.size(); i++){
            ss << ',' << cmd_params[i];
        }
        std::string payload = ss.str();
        publish(nullptr, "command", payload.size(), payload.c_str(), 2, false);
        return 0;
    }
};
