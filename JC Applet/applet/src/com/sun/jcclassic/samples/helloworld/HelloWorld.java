package com.sun.jcclassic.samples.helloworld;

import javacard.framework.APDU;
import javacard.framework.Applet;
import javacard.framework.ISO7816;
import javacard.framework.ISOException;
import javacard.framework.Util;

//import java.lang.*;
import javacard.security.*;
import javacardx.security.*;
import javacard.security.KeyBuilder;
import javacardx.crypto.Cipher;

//import javacard.framework.ISOException;

public class HelloWorld extends Applet {

	public final static boolean DEBUG = true;
	
	private byte[] RecievedStatic;
    private byte[] SendStatic;
    private byte[] AESPSKKey;
    
    private static final short LENGTH_ECHO_BYTES = 256;

    static final byte HW_AES_ENCRYPT = (byte)  0x03;
    static final byte HW_AES_DECRYPT = (byte)  0x04;
    static final byte HW_RSA_ENCRYPT = (byte)  0x05;
    static final byte HW_RSA_DECRYPT = (byte)  0x06;
    static final byte TEST_INPUTHEAD = (byte)  0xff;
    
    static final byte AES_MESSAGE_LGTH = (byte) 0x10;
    static final byte RSA_MESSAGE_LGTH = (byte) 0x11;

    static private Cipher AES_ECB;
    static private AESKey AES_Key;
    
    //private Cipher RSA_ECB;
    //private RSAKey RSA_Key;
    
    protected HelloWorld() {
    	RecievedStatic=	new byte[LENGTH_ECHO_BYTES];
    	SendStatic = 	new byte[LENGTH_ECHO_BYTES];
    	AESPSKKey =		new byte[] { (byte)0x2b, (byte)0x7e, (byte)0x15, (byte)0x16,
					    			 (byte)0x28, (byte)0xae, (byte)0xd2, (byte)0xa6,
					    			 (byte)0xab, (byte)0xf7, (byte)0x15, (byte)0x88,
					    			 (byte)0x09, (byte)0xcf, (byte)0x4f, (byte)0x3c };  

        register();
    }

    public static void install(byte[] bArray, short bOffset, byte bLength) {
        new HelloWorld();
    }

    public void SendResponse(APDU apdu, short lenght) {
    	apdu.setOutgoing();
        apdu.setOutgoingLength((short) (ISO7816.OFFSET_CDATA + lenght));
        
        apdu.sendBytes((short) 0, (short) 5);
        apdu.sendBytesLong(SendStatic, (short) 0, lenght);
    }
    
    public void AES_DUMMY(byte count, byte[] bMessage) {
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
    public void RSA_DUMMY(byte count, byte[] bMessage) {
    	for(byte iterator = (byte) 0; iterator < count; iterator++ ) {
    		SendStatic[iterator]=bMessage[(short)(count-iterator+(short)0)];
    	}
    }
    
   @Override 
   public void process(APDU apdu) {

	   if(!DEBUG) {
	   		try {
		   		AES_Key = (AESKey) KeyBuilder.buildKey(KeyBuilder.TYPE_AES_TRANSIENT_DESELECT, KeyBuilder.LENGTH_AES_128, false);
		    	AES_Key.setKey(AESPSKKey, (short) 0);
		        AES_ECB = Cipher.getInstance(Cipher.ALG_AES_BLOCK_128_ECB_NOPAD, false);
	   		}
	   		catch (CryptoException e) {
		   	    short reason = e.getReason();
		   	    ISOException.throwIt(reason);
	   		}
   		}
	   
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
	        		SendResponse(apdu, (short) 1);
	        		break;
		        
	        	case HW_AES_ENCRYPT:
		        	//Call encrypt over data 16 bytov
		        	if( Offset == AES_MESSAGE_LGTH + 1) {
		        		
		        		if(!DEBUG) {
			        		AES_ECB.init(AES_Key, HW_AES_ENCRYPT);
			        		AES_ECB.doFinal(RecievedStatic, (short) 0, (short) 16, SendStatic, (short) 0);
		        		}
		        		else {
			        		AES_DUMMY(AES_MESSAGE_LGTH, RecievedStatic);		        			
		        		}
		        		
		        		SendResponse(apdu, (short) AES_MESSAGE_LGTH);
		        	}
		        	break;
		        case HW_AES_DECRYPT:
		        	//Call decrypt over data 16 bytov
		        	if( Offset==AES_MESSAGE_LGTH + 1) {
		        		
		        		if(!DEBUG) {
		        			AES_ECB.init(AES_Key, HW_AES_DECRYPT);
		        			AES_ECB.doFinal(RecievedStatic, (short) 0, (short) 16, SendStatic, (short) 0);
		        		}
		        		else {
			        		AES_DUMMY(AES_MESSAGE_LGTH, RecievedStatic);		        			
		        		}
		        		SendResponse(apdu, (short) AES_MESSAGE_LGTH);
		        	}
		        	break;
		        	
		        case HW_RSA_ENCRYPT:
		        	//Call encrypt over data N bytov
		        	if( Offset==RSA_MESSAGE_LGTH + 1 ) {
		        		RSA_DUMMY(RSA_MESSAGE_LGTH, RecievedStatic);
		        		SendResponse(apdu, (short) RSA_MESSAGE_LGTH);
		        	}
		        	break;
		        case HW_RSA_DECRYPT:
		        	//Call decrypt over data N bytov
		        	if( Offset==RSA_MESSAGE_LGTH + 1 ) {
		        		RSA_DUMMY(RSA_MESSAGE_LGTH, RecievedStatic);
		        		SendResponse(apdu, (short) RSA_MESSAGE_LGTH);
		        	}
		        	break;
		        
		        default: 
		        	//return 0;
		        	//Throw EXCEPTION?
		        	for(byte iterator=0;iterator<Offset;iterator++){
		        		SendStatic[iterator] = RecievedStatic[iterator];
		        	}
		        	SendResponse(apdu, (short) 5);
		        	break;
	        }
	    }
    }
}
