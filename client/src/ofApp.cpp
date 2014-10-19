#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    client.connect("192.168.1.17",9093);
//    client.connect("localhost", 9093);
    client.addListener(this);
    
    bSendImage = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    if(bSendImage){
        currentImage.loadImage(toLoad);
        
        client.send( ofToString(currentImage.width) +":"+ ofToString( currentImage.height ) +":"+ ofToString( currentImage.type ) );
        
        ofBuffer buff = ofBufferFromFile(toLoad);
        client.sendBinary(buff);
//        client.sendBinary( currentImage );
        bSendImage = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( currentImage.isAllocated() ){
        currentImage.draw(0,0);
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
    cout<<"got message "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'a') {
        client.send("heatingDots:200");
        client.send("heatingTime:60");
    } else if (key == 't'){
        client.send("Esto es un ejemplo");
    }
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
    for (int i=0; i<dragInfo.files.size(); i++){
        string file = dragInfo.files[i];
        ofFile f(file);
        string ex = f.getExtension();
        std::transform(ex.begin(), ex.end(),ex.begin(), ::toupper);
        
        if ( ex == "JPG" || ex == "JPEG" || ex == "PNG" || ex == "GIF" ){
            toLoad = file;
            bSendImage = true;
        }
    }
}
