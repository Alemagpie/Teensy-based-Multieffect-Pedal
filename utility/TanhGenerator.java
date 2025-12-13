import java.io.FileWriter;
import java.io.IOException;

public class TanhGenerator {
    public static void main(String[] args) {
        int limit = 3;
        float step = 0.05f;
        int count = 0;
        
        if(args.length > 0) {
            step = Float.parseFloat(args[0]);
        } 

        try {
            FileWriter writer = new FileWriter("tanh.txt");

            for(float i = 0; i <= limit; i += step) {
                writer.write(Integer.toString((int)(Math.tanh(i)*32767)));
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