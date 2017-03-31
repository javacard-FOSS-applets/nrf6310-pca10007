package com.sun.jcclassic.samples.helloworld;

import javacard.framework.APDU;
import javacard.framework.Applet;
import javacard.framework.ISO7816;
import javacard.framework.Util;

//import java.lang.*;
//import javacard.security.*;
//import javacard.framework.ISOException;

public class HelloWorld extends Applet {
    private byte[] RecievedStatic;
    private byte[] SendStatic;
    private static final short LENGTH_ECHO_BYTES = 256;

    static final byte HW_AES_ENCRYPT = (byte)  0x03;
    static final byte HW_AES_DECRYPT = (byte)  0x04;
    static final byte HW_RSA_ENCRYPT = (byte)  0x05;
    static final byte HW_RSA_DECRYPT = (byte)  0x06;
    static final byte TEST_INPUTHEAD = (byte)  0xff;
    
    static final byte AES_MESSAGE_LGTH = (byte) 0x10;
    static final byte RSA_MESSAGE_LGTH = (byte) 0x11;

    protected HelloWorld() {
    	RecievedStatic = new byte[LENGTH_ECHO_BYTES];
    	SendStatic = 	 new byte[LENGTH_ECHO_BYTES];
        register();
    }

    
    public static void install(byte[] bArray, short bOffset, byte bLength) {
        new HelloWorld();
    }

    public void SendResponse(APDU apdu, short Offset, short lenght){
    	apdu.setOutgoing();
        apdu.setOutgoingLength((short) (5 + lenght));
        
        apdu.sendBytes((short) 0, (short) 5);
        apdu.sendBytesLong(SendStatic, (short) 0, lenght);
    }
    
    public void AES_DUMMY(byte count, byte[] bMessage){
    	for(byte iterator = (byte) 0; iterator < count; iterator++ ) {
    		if( bMessage[(short)(iterator+(short)1)] == (byte) 0x00 ) {
    			SendStatic[iterator] = (byte) 0x00 ;
    		}
    		else if( bMessage[(short)(iterator+(short)1)] == (byte) 0x55 ) {
    			SendStatic[iterator] = (byte) 0xAA ;
    		}
    		else if( bMessage[(short)(iterator+(short)1)] == (byte) 0xAA ) {
    			SendStatic[iterator] = (byte) 0x55 ;
    		}
    	}
    }
    
    public void RSA_DUMMY(byte count, byte[] bMessage){
    	for(byte iterator = (byte) 0; iterator < count; iterator++ ) {
    		SendStatic[iterator]=bMessage[(short)(count-iterator+(short)1)];
    	}
    }
    
   @Override 
   public void process(APDU apdu) {
        byte recieveBuffer[] = apdu.getBuffer();

        if ((recieveBuffer[ISO7816.OFFSET_CLA] == 0) &&
                (recieveBuffer[ISO7816.OFFSET_INS] == (byte) (0xA4))) {
            return;
        }

        short bytesToRead = apdu.setIncomingAndReceive();
        short Offset = (short) 0;
        short Security = (short) 0;
        
        while (bytesToRead > 0) {
            Util.arrayCopyNonAtomic(recieveBuffer, ISO7816.OFFSET_CDATA, RecievedStatic, Offset, bytesToRead);
            Offset += bytesToRead;
            bytesToRead = apdu.receiveBytes(ISO7816.OFFSET_CDATA);
        }
        
        Security=RecievedStatic[(short)(0)];
        
        if(Offset>=1) {
	        switch(Security) {
	        	case TEST_INPUTHEAD:
		        	for(byte iterator=0;iterator<Offset;iterator++){
		        		SendStatic[iterator] = RecievedStatic[iterator];
		        	}
	        		SendStatic[0]=(byte)0xAA;
	        		SendResponse(apdu, (short) Offset, (short) 1);
	        		break;
		        
	        	case HW_AES_ENCRYPT:
		        	//Call encrypt over data 16 bytov
		        	if( Offset == AES_MESSAGE_LGTH + 1) {
		        		AES_DUMMY(AES_MESSAGE_LGTH, RecievedStatic);
		        		SendResponse(apdu, (short) Offset, (short) AES_MESSAGE_LGTH);
		        	}
		        	break;
		        case HW_AES_DECRYPT:
		        	//Call decrypt over data 16 bytov
		        	if( Offset==AES_MESSAGE_LGTH  + 1) {
		        		AES_DUMMY(AES_MESSAGE_LGTH, RecievedStatic);
		        		SendResponse(apdu, (short) Offset, (short) AES_MESSAGE_LGTH);
		        	}
		        	break;
		        	
		        case HW_RSA_ENCRYPT:
		        	//Call encrypt over data N bytov
		        	if( Offset==RSA_MESSAGE_LGTH + 1 ) {
		        		RSA_DUMMY(RSA_MESSAGE_LGTH, RecievedStatic);
		        		SendResponse(apdu, (short) Offset, (short) RSA_MESSAGE_LGTH);
		        	}
		        	break;
		        case HW_RSA_DECRYPT:
		        	//Call decrypt over data N bytov
		        	if( Offset==RSA_MESSAGE_LGTH + 1 ) {
		        		RSA_DUMMY(RSA_MESSAGE_LGTH, RecievedStatic);
		        		SendResponse(apdu, (short) Offset, (short) RSA_MESSAGE_LGTH);
		        	}
		        	break;
		        
		        default: 
		        	//return 0;
		        	//Throw EXCEPTION?
		        	for(byte iterator=0;iterator<Offset;iterator++){
		        		SendStatic[iterator] = RecievedStatic[iterator];
		        	}
		        	SendResponse(apdu, Offset, (short) 5);
		        	break;
	        }
	    }
    }
}
