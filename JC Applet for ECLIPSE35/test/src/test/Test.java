package test;

import javacard.framework.APDU;
import javacard.framework.Applet;
import javacard.framework.ISO7816;
import javacard.framework.ISOException;
import javacard.framework.Util;

//import java.lang.*;
import javacard.security.*;
//import javacardx.security.*;
import javacard.security.KeyBuilder;
import javacardx.crypto.Cipher;
//import javacardx.crypto.*;

public class Test extends Applet {
	// ############################			CONSTATNTS			###########################
	public final static boolean DEBUG = false;
	
	public final static byte MASTER = 0x00;
	public final static byte SLAVE =  0x01;

	private static final short LENGTH_ECHO_BYTES = 256;

	private final static byte[] HELLO_STRING = new byte[]{'A', 'l', 'o', 'h', 'a'};
	
	static final byte HW_AES_ENCRYPT = (byte)  0x03;
    static final byte HW_AES_DECRYPT = (byte)  0x04;
    static final byte HW_RSA_ENCRYPT = (byte)  0x05;
    static final byte HW_RSA_DECRYPT = (byte)  0x06;
    static final byte HELLOWORLD	 = (byte)  'H';
    static final byte TEST_INPUTHEAD = (byte)  0xff;
    
    static final byte AES_MESSAGE_LGTH = (byte) 0x10;
    static final byte RSA_MESSAGE_LGTH = (byte) 0x11;

	
	//############################			CONFIG				###########################
	public static byte MS_Select = MASTER;
    static private byte app_initialized=(byte) 0;	
	
	//############################			VARIABLES			###########################
		//Buffers
	    	private static byte[] RecievedStatic = new byte[LENGTH_ECHO_BYTES];
	    	private static byte[] SendStatic = new byte[LENGTH_ECHO_BYTES];
    
    	//AES
	    	static private Cipher AES_ECB;
	    	static private AESKey AES_Key;
	    	private static byte[] AESPSKKey =	new byte[] { (byte)0x2b, (byte)0x7e, (byte)0x15, (byte)0x16,
												   			 (byte)0x28, (byte)0xae, (byte)0xd2, (byte)0xa6,
															 (byte)0xab, (byte)0xf7, (byte)0x15, (byte)0x88,
															 (byte)0x09, (byte)0xcf, (byte)0x4f, (byte)0x3c };  
    
/*
		//RSA
	    	private Cipher RSA_ECB;
    	//RSA Master
		    private RSAPrivateKey RSA_Master_Private;
		    private RSAPublicKey RSA_Master_Public;
    	//RSA Slave
		    private RSAPrivateKey RSA_Slave_Private;
		    private RSAPublicKey RSA_Slave_Public;
*/	
    
    public void Init() {
    	try {
	    	if(app_initialized==1) {
		    	//return;
	    	}
	//	    RecievedStatic=	new byte[LENGTH_ECHO_BYTES];
	//    	SendStatic = 	new byte[LENGTH_ECHO_BYTES];
	    	/*AESPSKKey =		new byte[] { (byte)0x2b, (byte)0x7e, (byte)0x15, (byte)0x16,
						    			 (byte)0x28, (byte)0xae, (byte)0xd2, (byte)0xa6,
						    			 (byte)0xab, (byte)0xf7, (byte)0x15, (byte)0x88,
						    			 (byte)0x09, (byte)0xcf, (byte)0x4f, (byte)0x3c };*/  
			app_initialized=1;
			
	   		AES_Key = (AESKey) KeyBuilder.buildKey(KeyBuilder.TYPE_AES_TRANSIENT_DESELECT, KeyBuilder.LENGTH_AES_128, false);
	    	AES_Key.setKey(AESPSKKey, (short) 0);
	        AES_ECB = Cipher.getInstance(Cipher.ALG_AES_BLOCK_128_CBC_NOPAD, false);
		}
		catch (ISOException e) {
	   	    short reason = e.getReason();
	   	    ISOException.throwIt(reason);
		}
		catch (CryptoException e) {
			short reason = e.getReason();
	   	    ISOException.throwIt(reason);
		}
	}

   /* private void Set_Master_Slave(byte selector) {
    
    }*/
    
	private Test() {
		Init();
	}

	public static void install(byte bArray[], short bOffset, byte bLength) throws ISOException {
		new Test().register();
	}

	public void Send(APDU apdu, short lenght) {
		byte buffer[] = apdu.getBuffer();
		Util.arrayCopyNonAtomic(SendStatic, (short) 0, buffer, (short) 0, (short) lenght);
        apdu.setOutgoingAndSend((short) 0, (short) lenght);
	}
	
    public void SendResponse(APDU apdu, short lenght) {
    	apdu.setOutgoing();
        apdu.setOutgoingLength((short) (ISO7816.OFFSET_CDATA + lenght));
        
        apdu.sendBytes((short) 0, (short) 5);
        apdu.sendBytesLong(SendStatic, (short) 0, lenght);
    	//apdu.setOutgoingAndSend((short) 0, lenght);
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
    
	public void process(APDU apdu) throws ISOException {
		/*Init();
		byte buffer[] = apdu.getBuffer();
        apdu.setIncomingAndReceive();
        Util.arrayCopyNonAtomic(HELLO_STRING, (short) 0, buffer, (short) 0, (short) HELLO_STRING.length);
        apdu.setOutgoingAndSend((short) 0, (short) HELLO_STRING.length);
	    
        return;
        */
	    
		byte recieveBuffer[] = apdu.getBuffer();
		
        if ((recieveBuffer[ISO7816.OFFSET_CLA] == 0) &&
                (recieveBuffer[ISO7816.OFFSET_INS] == (byte) (0xA4))) {
        	Init();
            return;
        }
        
        /*
        SendStatic[(short)0] = (byte) 0xaa;
        Send(apdu, (short)1);
        */
        
        
        short bytesToRead = apdu.setIncomingAndReceive();
        short Offset = (short) 0;
        byte Security = (short) 0;
        
        while (bytesToRead > 0) {
            Util.arrayCopyNonAtomic(recieveBuffer, ISO7816.OFFSET_CDATA, RecievedStatic, Offset, bytesToRead);
            Offset += bytesToRead;
            bytesToRead = apdu.receiveBytes(ISO7816.OFFSET_CDATA);
        }
        
        Security=RecievedStatic[(byte)(0)];
        
        SendStatic[(short)0] = (byte) Security;
        Send(apdu, (short)1);
        /*
        if(Offset>=1) {
	        switch(Security) {
	        	case HELLOWORLD:
	        		Util.arrayCopyNonAtomic(HELLO_STRING, (short) 0, SendStatic, (short) 0, (short) HELLO_STRING.length);
	        		SendResponse(apdu, (short) HELLO_STRING.length);

	        		break;
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
		        			try {
				        		AES_ECB.init(AES_Key, Cipher.MODE_ENCRYPT);
				        		AES_ECB.doFinal(RecievedStatic, (short) 1, (short) 16, SendStatic, (short) 0);
			        		}
			           		catch (ISOException e) {
			        	   	    short reason = e.getReason();
			        	   	    ISOException.throwIt(reason);
			           		}
			           		catch (CryptoException e) {
			        	   	    short reason = e.getReason();
			        	   	    ISOException.throwIt(reason);
			           		}
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
		        			try {
			        			AES_ECB.init(AES_Key, Cipher.MODE_DECRYPT);
			        			AES_ECB.doFinal(RecievedStatic, (short) 1, (short) 16, SendStatic, (short) 0);
		        			}
		        			catch (ISOException e) {
			        	   	    short reason = e.getReason();
			        	   	    ISOException.throwIt(reason);
			           		}
			           		catch (CryptoException e) {
			        	   	    short reason = e.getReason();
			        	   	    ISOException.throwIt(reason);
			           		}
		        		}
		        		else {
			        		AES_DUMMY(AES_MESSAGE_LGTH, RecievedStatic);		        			
		        		}
		        		SendResponse(apdu, (short) AES_MESSAGE_LGTH);
		        	}
		        	break;

		        /*case HW_RSA_ENCRYPT:
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
		        	break;*/
	/*	        default: 
		        	//return 0;
		        	//Throw EXCEPTION?
		        	for(byte iterator=0;iterator<Offset;iterator++){
		        		SendStatic[iterator] = RecievedStatic[iterator];
		        	}
		        	SendResponse(apdu, (short) Offset);
		        	break;
	        }
        }*/
	}
}
