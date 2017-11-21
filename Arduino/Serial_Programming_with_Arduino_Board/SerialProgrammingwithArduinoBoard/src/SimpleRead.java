import java.io.*;
import java.util.*;
//import javax.comm.*;	   // x64 운영체제의 경우 
import gnu.io.*;
import java.lang.*;

public class SimpleRead implements Runnable, SerialPortEventListener {

    InputStream inputStream;
    Thread readThread;
 
 // SimpleRead 생성자
    public SimpleRead() {
    	
        try {
            // 시리얼 포트에서 입력 스트림을 획득한다.
            inputStream = Serial_Commnunication.serialPort.getInputStream();
            
        } catch (IOException e) { }
        // 시리얼 포트의 이벤트 리스너로 자신을 등록한다.
        try {
        	Serial_Commnunication.serialPort.addEventListener(this);
        } catch (TooManyListenersException e) { }
        
        // 쓰레드 객체 생성
        readThread = new Thread(this);

        // 쓰레드 동작
        readThread.start();
    }
 
    public void run() {
    	try {
    		Thread.sleep(20000);
    	} catch (InterruptedException e) { }
    }
 // 시리얼 포트 이벤트가 발생하면 호출. 시리얼 포트 이벤트를 전달한다.
    public void serialEvent(SerialPortEvent event) {
        // 이벤트의 타입에 따라 switch 문으로 제어.
        switch (event.getEventType()) {
        case SerialPortEvent.BI:
        case SerialPortEvent.OE:
        case SerialPortEvent.FE:
        case SerialPortEvent.PE:
        case SerialPortEvent.CD:
        case SerialPortEvent.CTS:
        case SerialPortEvent.DSR:
        case SerialPortEvent.RI:
        case SerialPortEvent.OUTPUT_BUFFER_EMPTY:
            break;
            // 데이터가 도착하면
            case SerialPortEvent.DATA_AVAILABLE:
                byte[] readBuffer = new byte[300];    // byte 배열 객체 생성
                int numBytes=0;
                
                // 입력 스트림이 사용가능하면, 버퍼로 읽어 들인 후
                // String 객체로 변환하여 출력
                try {
                    while (inputStream.available() > 0) {
                        numBytes = inputStream.read(readBuffer);
                    }
                    System.out.print(new String(readBuffer,0,numBytes));
                } catch (IOException e) { }
                break;
            }
        }
    }
    