package tt;

import javacard.framework.*;

public class tt extends Applet
{
	private final static byte[] HELLO_WORLD = new byte[]{'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
	
	public static void install(byte[] bArray, short bOffset, byte bLength) 
	{
		new tt().register(bArray, (short) (bOffset + 1), bArray[bOffset]);
	}

	public void process(APDU apdu)
	{
		if (selectingApplet())
		{
			return;
		}

		byte buffer[] = apdu.getBuffer();
        apdu.setIncomingAndReceive();
        Util.arrayCopyNonAtomic(HELLO_WORLD, (short) 0, buffer, (short) 0, (short) HELLO_WORLD.length);
        apdu.setOutgoingAndSend((short) 0, (short) HELLO_WORLD.length);

	}

}
