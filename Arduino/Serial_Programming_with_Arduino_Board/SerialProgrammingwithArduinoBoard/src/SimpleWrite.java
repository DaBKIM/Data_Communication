import java.io.*;
import java.util.*;
import gnu.io.*;
import java.lang.*;

public class SimpleWrite{
	
	String tmp;
    OutputStream outputStream;
    
    
    public SimpleWrite() {
    	
        try {
            // 시리얼 포트에서 출력 스트림을 획득한다.
            outputStream = Serial_Commnunication.serialPort.getOutputStream();
            
        } catch (IOException e) { }
        
       while(true){
       	 try{
             
            // 반복적으로 Scanner를 받아 Input을 계속 넘겨준다.
       		Scanner in= new Scanner(System.in);
         	   	tmp=in.nextLine();
         	   	outputStream.write(tmp.getBytes());
             
             // 넘겨준 후
             // 아두이노에서 온 값을 바로 받을 수 있도록 Reader 호출.
         	   SimpleRead reader = new SimpleRead();
         	   
       	 }catch(IOException e){}
       }
        
    }
}
