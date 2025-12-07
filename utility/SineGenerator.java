import java.io.FileWriter;
import java.io.IOException;
import java.math.*;

public class SineGenerator {
    public static void main(String[] args) {
        float limit = (float) (2*Math.PI);
        float step = 0.015f;
        int count = 0;

        if(args.length > 0) {
            step = Float.parseFloat(args[0]);
        } 
        
        try {
            FileWriter writer = new FileWriter("sine.txt");

            for(float i = 0; i <= limit; i += step) {
                writer.write(Integer.toString((int)(Math.sin(i)*32767)));
                writer.write(", ");

                count++;
            }

            writer.write("\n \n (" + count + " samples)");

            writer.close();
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }
}