package tt;

import javacard.framework.*;

import javacard.framework.APDU;
import javacard.framework.Applet;
import javacard.framework.ISO7816;
import javacard.framework.ISOException;
import javacard.framework.Util;

//import java.lang.*;
import javacard.security.*;
import javacard.security.KeyBuilder;
//import javacard.security.;

public class tt extends Applet {
	private final static byte[] HELLO_WORLD = new byte[]{'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
	
	private static byte app_initialized=0;
	
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
    
    //static private Cipher AES_ECB;
    //static private AESKey AES_Key;
    
    public void Init() {
    	if(app_initialized==1) {
	    	//return;
    	}
	    RecievedStatic=	new byte[LENGTH_ECHO_BYTES];
    	SendStatic = 	new byte[LENGTH_ECHO_BYTES];
    	AESPSKKey =		new byte[] { (byte)0x2b, (byte)0x7e, (byte)0x15, (byte)0x16,
					    			 (byte)0x28, (byte)0xae, (byte)0xd2, (byte)0xa6,
					    			 (byte)0xab, (byte)0xf7, (byte)0x15, (byte)0x88,
					    			 (byte)0x09, (byte)0xcf, (byte)0x4f, (byte)0x3c };  
		app_initialized=1;
	}
    
	public static void install(byte[] bArray, short bOffset, byte bLength) {
		new tt().register(bArray, (short) (bOffset + 1), bArray[bOffset]);
	}

    public void SendResponse(APDU apdu, short lenght) {
    	apdu.setOutgoing();
        apdu.setOutgoingLength((short) (ISO7816.OFFSET_CDATA + lenght));
        
        apdu.sendBytes((short) 0, (short) 5);
        apdu.sendBytesLong(SendStatic, (short) 0, lenght);
    }

	public void process(APDU apdu) {
		//AESKey AES_Key;
		if (selectingApplet()) {
			Init();
			return;
		}
		
        byte recieveBuffer[] = apdu.getBuffer();
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
		        
	        }
		}
	}
}
