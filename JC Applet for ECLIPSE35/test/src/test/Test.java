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
	public final static boolean DEBUG = 	false;
	public final static boolean TYPE2SEND = true; // Conclusion IF YOU WANT TO INTERFACE ISO7816 
												  //BY YOURSELF, KEEP IT SIMPLE && DONT ASSEMBLE APDU BY YOUR SELF. USE ATOMI INSTRUCTION 
	public static boolean RSA_NOPAD = 		true;
	
	
	public final static byte MASTER = 0x00;
	public final static byte SLAVE =  0x01;

	private static final short LENGTH_ECHO_BYTES = 256;

	private final static byte[] HELLO_STRING = new byte[]{'A', 'l', 'o', 'h', 'a'};
	
	static final byte RSA_TYPE_CONFI = (byte)  0x00;
	
	static final byte HW_AES_ENCRYPT = (byte)  0x03;
    static final byte HW_AES_DECRYPT = (byte)  0x04;
    
    static final byte HW_RSA_ENCRYPT = (byte)  0x05;
    static final byte HW_RSA_DECRYPT = (byte)  0x06;
    
    static final byte GET_RANDOM = 	   (byte)  'R';
    static final byte HELLOWORLD	 = (byte)  'H';
    static final byte TEST_INPUTHEAD = (byte)  0xff;
    
    static final byte AES_MESSAGE_LGTH = (byte) 0x10;
    static final byte RSA_MESSAGE_LGTH = (byte) 128; //with 1024

	
	//############################			CONFIG				###########################
	public static byte MS_Select = MASTER;
    static private byte app_initialized=(byte) 0;	
	
	//############################			VARIABLES			###########################
		//Buffers
	    	private static byte[] RecievedStatic = new byte[LENGTH_ECHO_BYTES];
	    	private static byte[] SendStatic = new byte[LENGTH_ECHO_BYTES];
    
    	//AES
	    	static private Cipher AES_CBC;
	    	static private AESKey AES_Key;
	    	private static byte[] AESPSKKey =	new byte[] { (byte)0x2b, (byte)0x7e, (byte)0x15, (byte)0x16,
												   			 (byte)0x28, (byte)0xae, (byte)0xd2, (byte)0xa6,
															 (byte)0xab, (byte)0xf7, (byte)0x15, (byte)0x88,
															 (byte)0x09, (byte)0xcf, (byte)0x4f, (byte)0x3c };

		//RSA
	    	private Cipher 			RSA_ECB;
    	//RSA Master
		    private Key 			RSA_Master_Private;
		    private Key 			RSA_Master_Public;
		    private KeyPair 		RSA_Key_Pair;
		    
    	//RSA Slave
		   // private RSAPrivateKey RSA_Slave_Private;
		   // private RSAPublicKey RSA_Slave_Public;
	
    
	private void AES_init() {
		AES_Key = (AESKey) KeyBuilder.buildKey(KeyBuilder.TYPE_AES_TRANSIENT_DESELECT, KeyBuilder.LENGTH_AES_128, false);
    	AES_Key.setKey(AESPSKKey, (short) 0);
        AES_CBC = Cipher.getInstance(Cipher.ALG_AES_BLOCK_128_CBC_NOPAD, false);
	}
	
	private void RSA_init() {
		RSA_Master_Private =  KeyBuilder.buildKey(KeyBuilder.TYPE_RSA_PRIVATE, KeyBuilder.LENGTH_RSA_1024, false);
		RSA_Master_Public  =  KeyBuilder.buildKey(KeyBuilder.TYPE_RSA_PUBLIC,  KeyBuilder.LENGTH_RSA_1024, false);
		
		RSA_Key_Pair = new KeyPair(KeyPair.ALG_RSA, (short) RSA_Master_Private.getSize() );
		RSA_Key_Pair.genKeyPair();
		
		if(RSA_NOPAD) {
			RSA_ECB = Cipher.getInstance(Cipher.ALG_RSA_NOPAD, false);
		}
		else {
			RSA_ECB = Cipher.getInstance(Cipher.ALG_RSA_PKCS1, false);
		}
			 
		/* GP 2.2.2 supports this standards
		 	 Size of block, depends on the leght of the keysize 1024[b]/8=128[B] 
		 	 
			 ALG_RSA_ISO14888	-do not use, signature usage
			 ALG_RSA_ISO9796	-should not be used since 2000
			 ALG_RSA_PKCS1		-padding, scheme PKCS#1
			 ALG_RSA_PKCS1_OAEP	-padding, scheme PKCS#1-OAEP
			 ALG_RSA_NOPAD		-text based, not working, needs to be block alligned?
		 */
	}
	
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
			
	    	AES_init();
	    	RSA_init();
	    	
	    	app_initialized=1;
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
    
    private byte Get_Random() {
    	RandomData random_Generator;
    	random_Generator = RandomData.getInstance(RandomData.ALG_SECURE_RANDOM);
    	byte array[]= new byte[16];
    	random_Generator.generateData(array, (short)0, (short)1);
    	return array[0];
    }
    
	private Test() {
		Init();
	}

	public static void install(byte bArray[], short bOffset, byte bLength) throws ISOException {
		new Test().register();
	}

	public void Send_Type1(APDU apdu, short lenght) {
		apdu.setOutgoing();
        apdu.setOutgoingLength((short) (ISO7816.OFFSET_CDATA + lenght));
        
        apdu.sendBytes((short) 0, (short) 5);
        apdu.sendBytesLong(SendStatic, (short) 0, lenght);
    	//apdu.setOutgoingAndSend((short) 0, lenght);
	}
	
	public void Send_Type2(APDU apdu, short lenght) {
		byte buffer[] = apdu.getBuffer();
		Util.arrayCopyNonAtomic(SendStatic, (short) 0, buffer, (short) 0, (short) lenght);
        apdu.setOutgoingAndSend((short) 0, (short) lenght);
	}
	
    public void SendResponse(APDU apdu, short lenght) {
    	if(TYPE2SEND) {
    		Send_Type2(apdu, lenght);
    	}
    	else {
    		Send_Type1(apdu, lenght);
    	}
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
        SendResponse(apdu, (short)1);
        */
       
        short bytesToRead = apdu.setIncomingAndReceive();
        short Data_Lenght = (short) 0;
        byte 	Operation = (short) 0;
        
        while (bytesToRead > 0) {
            Util.arrayCopyNonAtomic(recieveBuffer, ISO7816.OFFSET_CDATA, RecievedStatic, Data_Lenght, bytesToRead);
            Data_Lenght += bytesToRead;
            bytesToRead = apdu.receiveBytes(ISO7816.OFFSET_CDATA);
        }
        
        Operation=RecievedStatic[(byte)(0)];
        
        /*SendStatic[(short)0] = (byte) Security;
        SendResponse(apdu, (short)1);*/
        
        if(Data_Lenght>=1) {
	        switch(Operation) {
	        	case HELLOWORLD:
	        		Util.arrayCopyNonAtomic(HELLO_STRING, (short) 0, SendStatic, (short) 0, (short) HELLO_STRING.length);
	        		SendResponse(apdu, (short) HELLO_STRING.length);

	        		break;
	        		
	        	case TEST_INPUTHEAD:
		        	for(byte iterator=0; iterator<Data_Lenght; iterator++){
		        		SendStatic[iterator] = RecievedStatic[iterator];
		        	}
	        		SendStatic[0]=(byte)0xAA;
	        		SendResponse(apdu, (short) 1);
	        		break;
	        		
	        	case HW_AES_ENCRYPT:
		        	//Call encrypt over data 16 bytov
		        	if( Data_Lenght == AES_MESSAGE_LGTH + 1) {
		        		
		        		if(!DEBUG) {
		        			try {
				        		AES_CBC.init(AES_Key, Cipher.MODE_ENCRYPT);
				        		AES_CBC.doFinal(RecievedStatic, (short) 1, (short) 16, SendStatic, (short) 0);
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
		        	if( Data_Lenght==AES_MESSAGE_LGTH + 1) {
		        		
		        		if(!DEBUG) {
		        			try {
			        			AES_CBC.init(AES_Key, Cipher.MODE_DECRYPT);
			        			AES_CBC.doFinal(RecievedStatic, (short) 1, (short) 16, SendStatic, (short) 0);
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

		        case HW_RSA_ENCRYPT:
		        	//Call encrypt over data N bytov
		        	try {
			        	RSA_ECB.init(RSA_Key_Pair.getPublic(), Cipher.MODE_ENCRYPT);
			        	short var;
			        	if(RSA_NOPAD) {
			        		var = RSA_ECB.doFinal(RecievedStatic, (short) 1, (short) 0x80, SendStatic, (short) 0);
			        		SendResponse(apdu, (short) var); //(Data_Lenght-1)
			        	}
			        	else {
			        		var = RSA_ECB.doFinal(RecievedStatic, (short) 1, (short) (Data_Lenght-1), SendStatic, (short) 0);
			        		SendResponse(apdu, (short) var);
			        	}
		        	}
		        	catch (ISOException e) {
	        	   	    short reason = e.getReason();
	        	   	    ISOException.throwIt(reason);
	           		}
	           		catch (CryptoException e) {
	        	   	    short reason = e.getReason();
	        	   	    ISOException.throwIt(reason);
	           		}
		        	break;
		        case HW_RSA_DECRYPT:
		        	//Call decrypt over data N bytov
		        	try {
			        	RSA_ECB.init(RSA_Key_Pair.getPrivate(), Cipher.MODE_DECRYPT);

			        	short var;
			        	
			        	if(RSA_NOPAD) {
			        		var = RSA_ECB.doFinal(RecievedStatic, (short) 1, (short) (Data_Lenght-1), SendStatic, (short) 0);
				        	SendResponse(apdu, (short) var); //(Data_Lenght-1)
			        	}
			        	else {
			        		var = RSA_ECB.doFinal(RecievedStatic, (short) 1, (short) (Data_Lenght-1), SendStatic, (short) 0);
			        		SendResponse(apdu, (short) var);
			        	}
		        	}
		        	catch (ISOException e) {
		    	   	    short reason = e.getReason();
		    	   	    ISOException.throwIt(reason);
		       		}
		       		catch (CryptoException e) {
		    	   	    short reason = e.getReason();
		    	   	    ISOException.throwIt(reason);
		       		}
				        	/*if( Offset==RSA_MESSAGE_LGTH + 1 ) {
		        		RSA_DUMMY(RSA_MESSAGE_LGTH, RecievedStatic);
		        		SendResponse(apdu, (short) RSA_MESSAGE_LGTH);
		        	}*/
		        	break;
		        	
		        case RSA_TYPE_CONFI:
		        	app_initialized=0;
		        	Init();
		        	
		        	if(RSA_NOPAD) {
		        		RSA_NOPAD=false;
		        		SendStatic[0] = (byte) 0;
		        	}
		        	else {
		        		RSA_NOPAD=true;
		        		SendStatic[0] = (byte) 1;
		        	}
		        	
		        	SendResponse(apdu, (short) 1);
		        	break;
		        	
		        case GET_RANDOM:
		        	SendStatic[0]=Get_Random();
	        		SendResponse(apdu, (short) 1);
	        		break;
	        		
		        default: 
		        	//return 0;
		        	//Throw EXCEPTION?
		        	for(byte iterator=0;iterator<Data_Lenght; iterator++){
		        		SendStatic[iterator] = RecievedStatic[iterator];
		        	}
		        	SendResponse(apdu, (short) Data_Lenght);
		        	break;
	        }
        }
	}
}
