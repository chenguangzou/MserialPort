///
/// \file 			SerialPort.hpp
/// \author 		Geoffrey Hunter <gbmhunter@gmail.com> ()
/// \created		2014-01-07
/// \last-modified 	2017-11-23
/// \brief			The main serial port class.
/// \details
///					See README.rst in repo root dir for more info.

// Header guard
#ifndef SERIAL_PORT_SERIAL_PORT_H
#define SERIAL_PORT_SERIAL_PORT_H

// System headers
#include <string>
#include <fstream> // For file I/O (reading/writing to COM port)
#include <sstream>
#include <androidLog.h>
#include <termios.h> // POSIX terminal control definitions (struct termios)
#include <vector>

// User headers

namespace mn {
    namespace CppLinuxSerial {

        static speed_t getBaudrate(int baudrate)
        {
            switch(baudrate) {
                case 0: return B0;
                case 50: return B50;
                case 75: return B75;
                case 110: return B110;
                case 134: return B134;
                case 150: return B150;
                case 200: return B200;
                case 300: return B300;
                case 600: return B600;
                case 1200: return B1200;
                case 1800: return B1800;
                case 2400: return B2400;
                case 4800: return B4800;
                case 9600: return B9600;
                case 19200: return B19200;
                case 38400: return B38400;
                case 57600: return B57600;
                case 115200: return B115200;
                case 230400: return B230400;
                case 460800: return B460800;
                case 500000: return B500000;
                case 576000: return B576000;
                case 921600: return B921600;
                case 1000000: return B1000000;
                case 1152000: return B1152000;
                case 1500000: return B1500000;
                case 2000000: return B2000000;
                case 2500000: return B2500000;
                case 3000000: return B3000000;
                case 3500000: return B3500000;
                case 4000000: return B4000000;
                default: return -1;
            }
        }
        enum class State {
            CLOSED,
            OPEN
        };


/// \brief		SerialPort object is used to perform rx/tx serial communication.
        class SerialPort {

        public:
            int getFileDescriptor();

            /// \brief		Default constructor. You must specify at least the device before calling Open().
            SerialPort();

            /// \brief		Constructor that sets up serial port with the basic (required) parameters.

            SerialPort(const std::string &device, int baudRate);

            SerialPort(const SerialPort& serialPort);

            //! @brief		Destructor. Closes serial port if still open.
            virtual ~SerialPort();

            /// \brief		Sets the device to use for serial port communications.
            /// \details    Method can be called when serial port is in any state.
            void SetDevice(const std::string &device);

            void SetBaudRate(int baudRate);

            /// \brief      Sets the read timeout (in milliseconds)/blocking mode.
            /// \details    Only call when state != OPEN. This method manupulates VMIN and VTIME.
            /// \param      timeout_ms  Set to -1 to infinite timeout, 0 to return immediately with any data (non
            ///             blocking, or >0 to wait for data for a specified number of milliseconds). Timeout will
            ///             be rounded to the nearest 100ms (a Linux API restriction). Maximum value limited to
            ///             25500ms (another Linux API restriction).
            void SetTimeout(int32_t timeout_ms);

            /// \brief		Enables/disables echo.
            /// \param		value		Pass in true to enable echo, false to disable echo.
            void SetEcho(bool value);

            /// \brief		Opens the COM port for use.
            /// \throws		CppLinuxSerial::Exception if device cannot be opened.
            /// \note		Must call this before you can configure the COM port.
            void Open();

            /// \brief		Closes the COM port.
            void Close();

            /// \brief		Sends a message over the com port.
            /// \param		data		The data that will be written to the COM port.
            /// \throws		CppLinuxSerial::Exception if state != OPEN.
            void Write(const std::string &data);

            /// \brief		Sends a message over the com port.
            /// \bytes		bytes		The data that will be written to the COM port.
            /// \len		bytes		Length of bytes to send
            /// \throws		CppLinuxSerial::Exception if state != OPEN.
            void Write(char *bytes, int len);

            /// \brief		Use to read from the COM port.
            /// \param		data		The object the read characters from the COM port will be saved to.
            /// \param      wait_ms     The amount of time to wait for data. Set to 0 for non-blocking mode. Set to -1
            ///                 to wait indefinitely for new data.
            /// \throws		CppLinuxSerial::Exception if state != OPEN.
            void Read(std::string &data);

            State currendState();

        private:

            /// \brief		Returns a populated termios structure for the passed in file descriptor.
            termios GetTermios();

            /// \brief		Configures the tty device as a serial port.
            /// \warning    Device must be open (valid file descriptor) when this is called.
            void ConfigureTermios();

            void SetTermios(termios myTermios);

            /// \brief      Keeps track of the serial port's state.
            State state_;

            /// \brief      The file path to the serial port device (e.g. "/dev/ttyUSB0").
            std::string device_;

            /// \brief      The current baud rate.
            /// \brief		The file descriptor for the open file. This gets written to when Open() is called.
            int fileDesc_;

            int custom_baudRate;

            bool echo_;

            int32_t timeout_ms_;

            std::vector<char> readBuffer_;
            unsigned char readBufferSize_B_;

            static constexpr int32_t defaultTimeout_ms_ = -1;
            static constexpr unsigned char defaultReadBufferSize_B_ = 255;


        };

    } // namespace CppLinuxSerial
} // namespace mn

#endif // #ifndef SERIAL_PORT_SERIAL_PORT_H
