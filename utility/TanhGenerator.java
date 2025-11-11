import java.io.FileWriter;
import java.io.IOException;
import java.math.*;

public class TanhGenerator {
    public static void main(String[] args) {
        int limit = 3;
        float step = 0.05f;
        
        try {
            FileWriter writer = new FileWriter("tanh.txt");

            for(float i = 0; i <= limit; i += step) {
                writer.write(Integer.toString((int)(Math.tanh(i)*32767)));
                writer.write(", ");
            }

            writer.close();
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }
}