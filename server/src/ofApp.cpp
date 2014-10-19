#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    // setup client @ port 9093
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9093;
    
    bool connected = server.setup( options );
    server.addListener(this);

    //  Set dev of the printer
#ifdef TARGET_RASPBERRY_PI
    printer.open("/dev/ttyAMA0");
#else
    printer.open("/dev/tty.PL2303-00002014");
#endif
    
    heatingDots=20;
    heatingTime=200;
    heatingInterval=250;
    
    printDensity=14;
    printBreakTime=4;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(commands.size()>0){
        mutex.lock();
        for (int i = commands.size() -1; i >= 0; i--) {
            vector<string> values = ofSplitString(commands[i], ":");
            
            if(values.size()==2){
                if(values[0] == "heatingDots"){
                    heatingDots = ofToInt(values[1]);
                    printer.setControlParameter(heatingDots,heatingTime,heatingInterval);
                    cout << values[0] << " set to " << heatingDots << endl;
                } else if (values[0] == "heatingTime"){
                    heatingTime = ofToInt(values[1]);
                    printer.setControlParameter(heatingDots,heatingTime,heatingInterval);
                    cout << values[0] << " set to " << heatingTime << endl;
                } else if (values[0] == "heatingInterval"){
                    heatingInterval = ofToInt(values[1]);
                    printer.setControlParameter(heatingDots,heatingTime,heatingInterval);
                    cout << values[0] << " set to " << heatingInterval << endl;
                } else if (values[0] == "printDensity"){
                    printDensity = ofToInt(values[1]);
                    printer.setPrintDensity(printDensity,printBreakTime);
                    cout << values[0] << " set to " << printDensity << endl;
                } else if (values[0] == "printBreakTime"){
                    printBreakTime = ofToInt(values[1]);
                    printer.setPrintDensity(printDensity,printBreakTime);
                    cout << values[0] << " set to " << printBreakTime << endl;
                } else if (values[0] == "sleepTime"){
                    printer.setSleepTime(ofToInt(values[1]));
                    cout << values[0] << " set to " << ofToInt(values[1]) << endl;
                } else if (values[0] == "doubleWidth"){
                    printer.setDoubleWidth(ofToBool(values[1]));
                    cout << values[0] << " set to " << ofToBool(values[1]) << endl;
                } else if (values[0] == "bold"){
                    printer.setBold(ofToBool(values[1]));
                    cout << values[0] << " set to " << ofToBool(values[1]) << endl;
                } else if (values[0] == "reverse"){
                    printer.setReverse(ofToBool(values[1]));
                    cout << values[0] << " set to " << ofToBool(values[1]) << endl;
                } else if (values[0] == "upDown"){
                    printer.setUpDown(ofToBool(values[1]));
                    cout << values[0] << " set to " << ofToBool(values[1]) << endl;
                } else if (values[0] == "underline"){
                    printer.setUnderline(ofToBool(values[1]));
                    cout << values[0] << " set to " << ofToBool(values[1]) << endl;
                } else if (values[0] == "keyPanel"){
                    printer.setKeyPanel(ofToBool(values[1]));
                    cout << values[0] << " set to " << ofToBool(values[1]) << endl;
                } else if (values[0] == "keyPanel"){
                    printer.setKeyPanel(ofToBool(values[1]));
                    cout << values[0] << " set to " << ofToBool(values[1]) << endl;
                }
            } else {
                cout << "Printing: " << commands[i] << endl;
                printer.println(commands[i]);
            }
            
            commands.erase(commands.begin()+i);
        }
        mutex.unlock();
    }
    
    // if we have some buffer data, load into our pixels
    if ( buff.size() != 0 ){
        mutex.lock();
        ofLoadImage(pixels, buff);
        
        printer.print(pixels);
        
        image.setFromPixels(pixels);
        
        buff.clear();
        mutex.unlock();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    if(image.isAllocated()){
        image.draw(0,0);
    }
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on connected";
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on open";
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on close";
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on idle";
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    mutex.lock();
    if(args.isBinary){
        buff.set(args.data.getBinaryBuffer(), args.data.size());
    } else {
        cout << "Message recive: " << args.message << endl;
        vector<string> newcommands = ofSplitString(args.data.getText(), "\n");
        if(newcommands.size()==0){
            for (int i = 0; i < newcommands.size(); i++) {
                commands.push_back(newcommands[i]);
            }
        } else {
            
            vector<string> image = ofSplitString(args.data.getText(), ":");
            if (image.size()!=3){
                commands.push_back(args.message);
            }
        }
        
    }
    mutex.unlock();
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
