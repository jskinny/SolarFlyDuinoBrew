#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	string host = Spacebrew::SPACEBREW_CLOUD; // "localhost";
    string name = "Solar Fly";
    string description = "It's amazing";
    
    spacebrew.addPublish("LightData", "range");
    spacebrew.connect( host, name, description );
    

    ofSetVerticalSync(true);
	
	bSendSerialMessage = false;
	ofBackground(255);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	font.loadFont("DIN.otf", 64);
	
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	int baud = 9600;
	serial.setup(0, baud); //open the first device
	//serial.setup("COM4", baud); // windows example
	//serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx example
	//serial.setup("/dev/ttyUSB0", baud); //linux example
	
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;
	memset(bytesReadString, 0, 4);
    
    ofSetFrameRate(60);
    

}

//--------------------------------------------------------------
void testApp::update(){
	
	if (bSendSerialMessage){
		
		// (1) write the letter "a" to serial:
		serial.writeByte('a');
		
		// (2) read
		// now we try to read 3 bytes
		// since we might not get them all the time 3 - but sometimes 0, 6, or something else,
		// we will try to read three bytes, as much as we can
		// otherwise, we may have a "lag" if we don't read fast enough
		// or just read three every time. now, we will be sure to
		// read as much as we can in groups of three...
		
		nTimesRead = 0;
		nBytesRead = 0;
		int nRead  = 0;  // a temp variable to keep count per read
		
		unsigned char bytesReturned[3];
		
		memset(bytesReadString, 0, 4);
		memset(bytesReturned, 0, 3);
		
		while( (nRead = serial.readBytes( bytesReturned, 3)) > 0){
			nTimesRead++;
			nBytesRead = nRead;
		};
        
        float fbytesReadString = ofToFloat(bytesReadString);
        
        fbytesReadString = ofMap(fbytesReadString, 0, 1023, 10, 12);

		memcpy(bytesReadString, bytesReturned, 3);
		
		readTime = ofGetElapsedTimef();
        
        if ( spacebrew.isConnected() ){
        float LightData = ofToFloat(bytesReadString);
        

            spacebrew.sendRange("LightData", (LightData) );
	}
    }}

//--------------------------------------------------------------
void testApp::draw(){
	if (nBytesRead > 0 && ((ofGetElapsedTimef() - readTime) < 0.5f)){
		ofSetColor(0);
	} else {
		ofSetColor(220);
	}
	string msg;

	msg += "solar light level: " + ofToString(bytesReadString) + "\n";
	font.drawString(msg, 50, 100);
}

//--------------------------------------------------------------
void testApp::onMessage( Spacebrew::Message & m ){
    if ( m.name == "datadisplay" ){
        bSendSerialMessage = m.valueBoolean();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (bSendSerialMessage == true) {
        bSendSerialMessage = false;
        spacebrew.sendBoolean("button", false);
    } else {
        bSendSerialMessage = true;
        spacebrew.sendBoolean("button", true);
    };

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}

