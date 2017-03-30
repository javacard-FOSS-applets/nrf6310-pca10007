package com.sun.jcclassic.samples.helloworld;

import javacard.framework.APDU;
import javacard.framework.Applet;
import javacard.framework.ISO7816;
import javacard.framework.Util;

//import java.lang.*;
//import javacard.security.*;
//import javacard.framework.ISOException;

public class HelloWorld extends Applet {
    private byte[] SendBuffer;
    private static final short LENGTH_ECHO_BYTES = 256;

    static final byte HW_AES_ENCRYPT = 0x03;
    static final byte HW_AES_DECRYPT = 0x04;
    static final byte HW_RSA_ENCRYPT = 0x05;
    static final byte HW_RSA_DECRYPT = 0x06;

    protected HelloWorld() {
    	SendBuffer = new byte[LENGTH_ECHO_BYTES];
        register();
    }

    public static void install(byte[] bArray, short bOffset, byte bLength) {
        new HelloWorld();
    }

    public void SendResponse(APDU apdu, short Offset, short lenght){
    	apdu.setOutgoing();
        apdu.setOutgoingLength((short) (Offset + 5));
        
        apdu.sendBytes((short) 0, (short) lenght);
        apdu.sendBytesLong(SendBuffer, (short) 0, lenght);
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
            Util.arrayCopyNonAtomic(recieveBuffer, ISO7816.OFFSET_CDATA, SendBuffer, Offset, bytesToRead);
            Offset += bytesToRead;
            bytesToRead = apdu.receiveBytes(ISO7816.OFFSET_CDATA);
        }
        
        Security=SendBuffer[(short)(0)];//Offset
        
        if(Offset>=1) {
	        switch(Security) {
	        	case (byte) 0xFF:
	        		SendBuffer[0]=(byte)0xAA;
	        		SendResponse(apdu, Offset, (short) 1);
	        		break;
		        case HW_AES_ENCRYPT:
		        	//Call encrypt over data 16 bytov
		        	break;
		        case HW_AES_DECRYPT:
		        	//Call decrypt over data 16 bytov
		        	break;
		        case HW_RSA_ENCRYPT:
		        	//Call encrypt over data N bytov
		        	break;
		        case HW_RSA_DECRYPT:
		        	//Call decrypt over data N bytov
		        	break;
		        default: 
		        	//return 0;
		        	//Throw EXCEPTION?
		        	SendResponse(apdu, Offset, (short) 5);
		        	break;
	        }
	    }
        
    }
}
