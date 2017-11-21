import java.io.*;
import java.util.*;
import gnu.io.*;
import java.lang.*;

public class SimpleWrite{
	
	String tmp;
    OutputStream outputStream;
    
    
    public SimpleWrite() {
    	
        try {
            outputStream = Serial_Commnunication.serialPort.getOutputStream();
            
        } catch (IOException e) { }
        
       while(true){
       	 try{
       		Scanner in= new Scanner(System.in);
         	   	tmp=in.nextLine();
         	   	outputStream.write(tmp.getBytes());
         	   	
         	   SimpleRead reader = new SimpleRead();
         	   
       	 }catch(IOException e){}
       }
        
    }
}