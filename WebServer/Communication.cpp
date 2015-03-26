/* This code is written to set up the serial communication between
* Windows PC and ESP8266 ESP-01 Wifi unit
* January 27th 2015     Yang Wu   */

#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;
using namespace System::Text::RegularExpressions;

public ref class PortChat
{
private:
	static int count = 0;
	static bool _continue;
	static SerialPort^ _serialPort;
	static String^	_comPort = "COM4";
	static String^  _API_KEY = "GPVP0E6QQVWU47LZ";
	static Semaphore^ _message_sema = gcnew Semaphore(1,1);
	static bool		_ready = true;
	static String^		_TCPid;

public:
	static void Main()
	{
		String^ message;
		array<Char>^ deliminator = { ' ' };
		String^ content;
		array<String^>^ tokens;
		StringComparer^ stringComparer = StringComparer::OrdinalIgnoreCase;
		Thread^ readThread = gcnew Thread(gcnew ThreadStart(PortChat::Read));
		//Thread^ webThread = gcnew Thread(gcnew ThreadStart(PortChat::webServer));

		// Create a new SerialPort object with default settings.
		_serialPort = gcnew SerialPort(_comPort, 9600);

		// Set the read/write timeouts
		_serialPort->ReadTimeout = 2000;
		_serialPort->WriteTimeout = 2000;

		_serialPort->Open();
		_continue = true;
		readThread->Start();
		//webThread->Start();
		while (_continue)
		{
			message = Console::ReadLine();

			if (stringComparer->Equals("QUIT", message))
			{
				_continue = false;
			}
			else if (stringComparer->Equals("WEB", message))
			{
				webServer();
			}
			else if (stringComparer->Equals("TCP", message))
			{
				message = "AT+CIPSTART=1,\"" + message + "\",\"184.106.153.149\",80";
				_serialPort->WriteLine(message + "\r");
			}
			else if (stringComparer->Equals("REQUEST", message))
			{
				Console::WriteLine("Please enter: GET/SET:");
				message = Console::ReadLine();
				if (stringComparer->Equals("GET", message))
				{
					content = "GET /channels/26355/feeds.json?results=1\r\n";
				}
				else
				{
					tokens = message->Split(deliminator);
					Console::WriteLine("{0}    {1}", tokens[0], tokens[1]);
					content = "GET /update?key=";
					content += _API_KEY;
					content += "&";
					content += "field1=";
					content += tokens[1];
					content += "\r\n";
				}
				
				message = "AT+CIPSEND=1," + (content->Length-1);

				_ready = false;
				_message_sema->WaitOne();
				_serialPort->WriteLine(message + "\r");
			
				_message_sema->WaitOne();
				_serialPort->WriteLine(content);
				_message_sema->Release();		
			}
			else
			{
				//message = message->ToUpper();
				Console::WriteLine("<" + message + ">");
				_serialPort->WriteLine(message+"\r");
			}
		}

		//webThread->Join();
		readThread->Join();
		_serialPort->Close();
	}

	static void Read()
	{
		while (_continue)
		{
			try
			{
				
				String^ message = _serialPort->ReadLine();

				if (!_ready)
				{
					_ready = true;
					_message_sema->Release();		
				}
				
				Console::WriteLine(message);

				if (message->Contains("IPD")) /*truncate the Channel ID from the HTTP response*/
				{
					String^ pattern = "IPD,[0-9]*"; /* The string pattern */
					//Console::WriteLine("pattern : '{0}'", pattern);
					Regex^ regex = gcnew Regex(pattern);

					for (Match^ match = regex->Match(message);
						match->Success; match = match->NextMatch())
					{
						if (match->Value->Length > 0)
						{
							Console::WriteLine("{0}", match->Value);
							_TCPid = Regex::Match(match->Value, "[0-9]")->Value;
							break; /*Only look for the first match result*/
						}
					}
					Thread^ webThread = gcnew Thread(gcnew ThreadStart(PortChat::webServer)); /* Create a thread deal with the HTTP response */
					webThread->Start();
				}
			}
			catch (TimeoutException^) { 
				//Console::WriteLine("."); 
			}
		}
	}

	static void webServer()
	{
		System::Threading::Thread::Sleep(TimeSpan::FromSeconds(5));
		String^ msg = "AT+CIPSEND=";
		String^ resp = "<h1> ESP8266 Web Server</h1>\n";
		resp += "<p>GPIO0 <a href=\"?pin=ON1\"><button>ON</button></a>&nbsp;<a href=\"?pin=OFF1\"><button>OFF</button></a></p>";
		resp += "<p>GPIO2 <a href=\"?pin=ON2\"><button>ON</button></a>&nbsp;<a href=\"?pin=OFF2\"><button>OFF</button></a></p>";
		msg += _TCPid;
		msg += ",";
		msg += (resp->Length);

		_ready = false;
		_message_sema->WaitOne(); /*set the semaphore*/
		_serialPort->WriteLine(msg + "\r");
				
				
		_message_sema->WaitOne(); /*Wait for the Read thread to release the semaphore*/
		_serialPort->WriteLine(resp+ "\r");
				
		_ready = false;
		_message_sema->WaitOne(); /*Wait for the Read thread to release this semaphore*/
		System::Threading::Thread::Sleep(TimeSpan::FromMilliseconds(1500));
		_serialPort->WriteLine("AT+CIPCLOSE=" + _TCPid + "\r");
		_message_sema->Release();	
		
	}

};

int main()
{
	PortChat::Main();
}