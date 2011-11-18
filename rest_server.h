#ifndef __Restful_server_h__
#define __Restful_server_h__

#include "WProgram.h"
#include "config_rest.h"
#include <utility/message.h>
#include <../streaming/Streaming.h>
#include <Ethernet.h>
#include <string.h>
#include "Print.h" 
#include "Stream.h" 

class RestServer {

	private:
		Message request;					// Current request message
		
		/* Current state of RestServer - -1: waiting for client, 0: reading verb, 
					1: reading request, 2: parsing request, 3: processing request, 
					4: sending response, 5: cleaning up for next request */	
		int process_state;
		
		/* Current request types - 0: GET request, 1: POST request */
		int request_type;				

		char eol_sequence[EOL_LENGTH + 1];			// request end sequence match chars
		char eoh_sequence[EOH_LENGTH + 1];			// request end sequence match chars
		char div_chars[DIV_ELEMENTS + 1];			// element division chars

		long timeout_start_time;					// timeout timer start time
		int timeout_period;							// timeout timer interval period
		
		boolean post_ready_to_read;
		boolean post_length_found;
		boolean post_length_read;
		int post_length_expected;
		int post_length_actual;

		boolean read_request(char);
		void get_verb(char);
		void parse_request();
		void process();
		void send_response(Stream &_client);
		void prepare_for_next_client();

		void check_timer();

		void read_services();
		int service_match(int, int);
		int state_match(int, int);

		void get_form(int, Stream &_client);

		int next_element(int);
		int check_for_state_msg(int);
		int check_start(int);
		int check_start_single(int);
		boolean div_found(char);
		boolean eol_found(char);
		boolean eoh_match_found(char);
		boolean length_match_found(char);
		
	public:
		#define WAITING				-1
		#define READ_VERB			 0
		#define READ_RESOURCE		 1	
		#define PARSE				 2
		#define PROCESS				 3
		#define RESPOND				 4
		#define RESET				 5

		struct Resource {
			int state;
			boolean get;
			boolean post;
		};
		
		struct Resource resources[SERVICES_COUNT];

		RestServer();								// constructor
		void respond();								// notifies rest_server when ready to respond
		boolean handle_requests(Stream &_client); 	// reads request from Ethernet client
		boolean handle_response(Stream &_client); 	// sends response to Ethernet client

};



#endif // endif __Restful_server_h__
