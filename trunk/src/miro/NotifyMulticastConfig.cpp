// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast Config
//
//
//  (c) 2002, 2003
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <philipp.baer@informatik.uni-ulm.de>
//    Hans Utz <hans.utz@informatik.uni-ulm.de>
//
//
//  Version:
//    1.0.4
//
//
//  Todo:
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

/* NotifyMulticast includes */
#include "NotifyMulticastDefines.h"
#include "NotifyMulticastConfig.h"
#include "NotifyMulticastParameters.h"

#include <sstream>

#define DBG_CLASSNAME "NotifyMulticast::Config"

namespace Miro {

    namespace NotifyMulticast {

        const char Config::months[13][4] = {
                                               "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                               "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
                                           };

        /**
         * Config::Config()
         *
         *   Description:
         *     Default constructor
         */
        Config::Config() :
                eventChannel_(),
                socketAddress_(),
                socket_(new ACE_SOCK_Dgram_Mcast()),
                domain_(),
                subscribed_(false),
        logfile_() {
            Parameters *parameters = Parameters::instance();

            std::cout << "NotifyMulticast Logfile: " << parameters->logfile << std::endl;
            /* setup logfile */

            if (parameters->logfile.length() > 0) {
                logfile_.open(parameters->logfile.c_str());

                if (!logfile_.is_open()) {
                    PRINT_ERR("Error opening/creating logfile " << parameters->logfile);
                    useLogfile_ = false;
                } else
                    useLogfile_ = true;
            }

        }


        /**
         * Config::~Config()
         *
         *   Description:
         *     Default destructor
         */
        Config::~Config() {
            socket_->unsubscribe(socketAddress_);
            subscribed_ = false;
        }


        /**
         * Config::setEventchannel()
         *
         *   Description:
         *     Set event channel
         *
         *   Parameters:
         *     _eventChannel: Pointer to eventchannel (copy)
         */
        void Config::setEventchannel(CosNotifyChannelAdmin::EventChannel_ptr _eventChannel) {
            eventChannel_ = CosNotifyChannelAdmin::EventChannel::_duplicate(_eventChannel);
        }


        /**
         * Config::getEventchannel()
         *
         *   Description:
         *     Returns pointer to eventchannel
         */
        CosNotifyChannelAdmin::EventChannel_ptr Config::getEventchannel() {
            return eventChannel_.in();
        }


        /**
         * Config::setSocket()
         *
         *   Description:
         *     Set socket for communication
         *
         *   Parameters:
         *     _address: Multicast address
         *
         *   Throws:
         *     Miro::Exception
         */
        void Config::setSocket(ACE_INET_Addr &_address) throw(Miro::Exception) {
            socketAddress_ = _address;

            if (subscribed_) {
                socket_->unsubscribe(socketAddress_);
                subscribed_ = false;
            }

            if (socket_->subscribe(socketAddress_) == -1) {
                PRINT_ERR("Cannot subscribe multicast address");
                throw Miro::Exception("Cannot subscribe multicast address");
            }

            subscribed_ = true;
        }


        /**
         * Config::getSocket()
         *
         *   Description:
         *     Returns pointer to socket object
         */
        ACE_SOCK_Dgram_Mcast *Config::getSocket() {
            return socket_;
        }


        /**
         * Config::getSocketAddress()
         *
         *   Description:
         *     Returns address objects containing the multicast address specified before
         */
        ACE_INET_Addr &Config::getSocketAddress() {
            return socketAddress_;
        }


        /**
         * Config::setDomain()
         *
         *   Description:
         *     Set domainname
         *
         *   Parameters:
         *     _domain: domainname
         */
        void Config::setDomain(const std::string _domain) {
            domain_ = _domain;
        }


        /**
         * Config::getDomain()
         *
         *   Description:
         *     Returns domainname
         */
        const std::string Config::getDomain() {
            return domain_;
        }


        /**
         * Config::setEventMaxAge()
         *
         *   Description:
         *     Set ttl for events that arrive. If an event is too old,
         *     it is discarded.
         *
         *   Parameters:
         *     _eventMaxAge: max age in milliseconds.
         */
        void Config::setEventMaxAge(unsigned long int _eventMaxAge) {
            eventMaxAge_ = _eventMaxAge;
        }


        /**
         * Config::getEventMaxAge()
         *
         *   Description:
         *     Returns max age value of events that arrive
         */
        unsigned long int Config::getEventMaxAge() {
            return eventMaxAge_;
        }


        /**
         * Config::logfile()
         *
         *   Description:
         *     Return pointer to logfile
         */
        ofstream *Config::logfile() {
            return (useLogfile_ ? &logfile_ : NULL);
        }


        std::string Config::datetime() {
            std::stringstream str;

            dt_.update();

            str << months[dt_.month()] << " "
            << (dt_.day() < 10 ? "0" : "") << dt_.day() << " "
            << (dt_.hour() < 10 ? "0" : "") << dt_.hour() << ":"
            << (dt_.minute() < 10 ? "0" : "") << dt_.minute() << ":"
            << (dt_.second() < 10 ? "0" : "") << dt_.second() << ": ";

            return str.str();
        }

    };
};
