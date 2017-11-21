import java.io.*;
import java.util.*;
//import javax.comm.*;	   // x64 운영체제의 경우 
import gnu.io.*;
import java.lang.*;

public class Serial_Commnunication {
	
	static CommPortIdentifier portId;
    static Enumeration portList;
    static SerialPort serialPort;
	
	public static void main(String[] args) {
		
        // 시스템에 있는 가능한 드라이버의 목록을 받아온다.
        portList = CommPortIdentifier.getPortIdentifiers();

        // enumeration type 인 portList 의 모든 객체에 대하여
        while (portList.hasMoreElements()) {
            // enumeration 에서 객체를 하나 가져온다.
            portId = (CommPortIdentifier) portList.nextElement();
            
            // 가져온 객체의 port type 이 serial port 이면
            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
				if (portId.getName().equals("/dev/tty.usbmodem1421")) {
					
					try {
			            serialPort = (SerialPort)portId.open("Serial_CommnunicationApp", 2000);
			        } catch (PortInUseException e) {}
					
					// 시리얼 통신 설정. Data Bit는 8, Stop Bit는 1, Parity Bit는 없음.
			        try {
			        	serialPort.setSerialPortParams(9600,SerialPort.DATABITS_8, SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
			        	//serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
			        	
			        } catch (UnsupportedCommOperationException e) { }
			        
			        /* 시리얼 포트에 데이터가 도착하면 이벤트가 한 번 발생되는데
			           이 때, 자신이 리스너로 등록된 객체에게 이벤트를 전달하도록 허용. */
			       Serial_Commnunication.serialPort.notifyOnDataAvailable(true);
			        
			        SimpleWrite writer = new SimpleWrite();
				}
            }
        }
        
    }
}
