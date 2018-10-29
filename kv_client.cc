/*
* @author   Eashan Adhikarla
* @version  3.3
* @since    2018-10-09
*
* Class : Advance Programming Techniques
* Project 3 - Key Value Store - Client
*/

#include <iostream>
#include <libgen.h>
#include <unistd.h>
#include <boost/asio.hpp>
#include <fstream>

#define BUF_SIZE 4096

using namespace std;
/**
 * Connect to a server so that we can have bidirectional communication on the 
 * socket (represented by a file descriptor) that this function returns
 *
 * @param hostname The name of the server (ip or DNS) to connect to 
 * @param port the server's port that we should use
 */

boost::asio::ip::tcp::socket connect_to_server(std::string hostname,
 std::string port) {
  using namespace boost::asio;
  
  io_service io_service;
  ip::tcp::resolver resolver(io_service);
  ip::tcp::resolver::query query(hostname, port);
  ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  ip::tcp::socket socket(io_service);
  connect(socket, endpoint_iterator);
  return socket;
}

/**
 * Receive text from stdin, send it to the server, and then print whatever the
 * server sends back.
 * 
 * @param socket The socket file descriptor to use for the echo operation
 */
void echo_client(boost::asio::ip::tcp::socket &socket){
  using namespace std;
  using namespace boost::asio;

  // track connection duration, bytes transmitted
  size_t xmitBytes = 0;
  struct timeval start_time, end_time;
  gettimeofday(&start_time, nullptr);

  while(true){
    // Get the data from stdin, This assumes that we haven't redirected stdin to a socket
    cout<<"Client: ";
    string data, data_;
    getline(cin, data);
    if (cin.eof()){
      break;
    } 
    string key,value,str_length_key,str_length_value;
    int length_key,length_value;

    // Implementation of PUT function.
    if (data == "PUT")
    {
      cout << "********** Key Length ********** : ";
      getline(cin, str_length_key);
      length_key = stoi(str_length_key);
      cout << "********** Key ********** : ";
      getline(cin, key);
      if(key.length() != length_key)
      {
        cout << "********** Length Error **********" << endl;
        continue;
      }
      cout << "********** Value Length ********** : ";
      getline(cin, str_length_value);
      length_value = stoi(str_length_value);
      cout << "********** Value ********** : ";
      getline(cin, value);
      if(value.length() != length_value)
      {
        cout << "********** Length Error **********" << endl;
        continue;
      }
      //cout << "Checkpoint0";
      data = "PUT | " + str_length_key + " | " + key + " | " + str_length_value + " | " + value;
      data_ = key + " | " + value;

      ofstream out("kv_hashed.log", ios::app);
      if (out.is_open())
      {
        out << data_ << endl;
        out.close();
      }
    }

    // Implementation of GET function.
    else if (data == "GET")
    {
      cout << "********** Key Length ********** : ";
      getline(cin, str_length_key);
      length_key = stoi(str_length_key);
      cout << "********** Key ********** : ";
      getline(cin, key);
      if(key.length() != length_key)
      {
        cout << "********** Length Error **********" << endl;
        continue;
      }
      //data = "GET | " + str_length_key + " | " + key;
      data = "GET | " + str_length_value + " | " + value;
    }

    // Implementation of DEL function.
    else if (data == "DEL")
    {
      cout << "********** Key Length ********** : ";
      getline(cin, str_length_key);
      length_key = stoi(str_length_key);
      cout << "********** Key ********** : ";
      getline(cin, key);
      if(key.length() != length_key)
      {
        cout << "********** Length Error **********" << endl;
        continue;
      }
      data = "DEL | " + str_length_key + " | " + key;
    }

    // Implementation of WHERE function.
    else if (data == "WHERE")
    {
      cout << "********** Regular Expression Length ********** : ";
      getline(cin, str_length_key);
      length_key = stoi(str_length_key);
      cout << "********** Regular Expression ********** : ";
      getline(cin, key);
      if(key.length() != length_key)
      {
        cout << "********** Length Error **********" << endl;
        continue;
      }
      data = "WHERE | " + str_length_key + " | " + key;
    }

    else if (data == "SHOW")
      data = "SHOW";

    // Implementation of REDUCE function.
    else if (data == "REDUCE")
    {
      /* code */
    }

    else if (data == "help"){
      cout << "\n########## // \e[1mFUNCTIONS\e[0m // ##########\n\n";
      cout << "\e[1mGET\e[0m    : Returns the value associated with a key.\n";
      cout << "\e[1mPUT\e[0m    : Adds a mapping from key to value, if no prior mapping for key exists.\n";
      cout << "\e[1mDEL\e[0m    : Removes key and its associated value from the store, and returns true.\n\n";
      cout << "########## // \e[1mMETHODS\e[0m // ##########\n\n";
      cout << "\e[1mWHERE\e[0m  : Returns all of the key/value pairs in the data set for which the key matches the regular expression provided by regexp.\n";
      cout << "\e[1mREDUCE\e[0m : Runs the provided JavaScript code and returns the result.\n";

      cout << "\e[1mexit\e[0m followed by \e[1mCtrl+C\e[0m : To exit the client and server end.\n\n";
    }

    // Recieve data from server side.
    size_t recd = 0;
    size_t len, position;
    stringstream stream;
    string str_buf;
    unsigned char buf[BUF_SIZE];
    boost::system::error_code ignored_error;
    write(socket, buffer(data), ignored_error);
    xmitBytes += data.length();

    cout << "Server: ";
    str_buf = "";
    //cout << "Checkpoint1.0";
    while (1){
      len = socket.read_some(boost::asio::buffer(buf), ignored_error);
      //cout << "Checkpoint2";
      
      for (int i = 0; i < len; ++i){
        stream << buf[i];
      }
    
      str_buf = stream.str();
      position = str_buf.find("\n");
    
      if ( position < str_buf.length() ){
        recd = stoi(str_buf.substr(0, position));
        break;
        }
    }
    //cout << "Checkpoint3";
    stream.str("");

    while(str_buf.length()<recd+1+to_string(recd).length()){
      len = socket.read_some(boost::asio::buffer(buf), ignored_error);
      for (int i = 0; i < len; ++i){
        stream << buf[i];
      }
    
      str_buf = str_buf + stream.str();
      stream.str("");
    }
    position = str_buf.find("\n");
    str_buf = str_buf.substr(position+1);
    cout << str_buf << endl;
  }
  gettimeofday(&end_time, nullptr);
  cout<<"\nTransmitted "<<xmitBytes<<" bytes in "<<(end_time.tv_sec - start_time.tv_sec)<<" seconds\n";
}


/** Print some helpful usage information */
void usage(const char *progname) {
  using std::cout;
  cout << "  Usage: " << progname << " [options]\n";
  cout << "    -p <int> : Port on which to listen (default 41100)\n";
  cout << "    -h       : print this message\n";
}

int main(int argc, char *argv[]) {
  // Config vars that we get via getopt
  string port = "9000";       // random seed
  bool show_help = false; // show usage?
  string server_name = "localhost";
  // Parse the command line options:
  int o;
  while ((o = getopt(argc, argv, "p:h")) != -1) {
    switch (o) {
      case 'h':
      show_help = true;
      break;
      case 'p':
      port = string(optarg);
      break;
      default:
      show_help = true;
      break;
    }
  }

  // Print help and exit
  if (show_help) {
    usage(basename(argv[0]));
    exit(0);
  }

  // Print the configuration... this makes results of scripted experiments
  // much easier to parse
  
  //client
  try {
    // Set up the client socket
    auto socket = connect_to_server(server_name, port);
    cout << "Connected with " << server_name<<".\n";
    cout << "Type 'help' for functions and relavant methods.\n\n";

    //Start the 'echo' interaction
    echo_client(socket);

    // clean up when done
    socket.close();
  }catch (std::exception &e){
    std::cerr << e.what() << std::endl;
  }
  return 0;
}


