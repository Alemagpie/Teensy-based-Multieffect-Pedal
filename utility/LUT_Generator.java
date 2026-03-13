import java.io.FileWriter;
import java.io.IOException;

public class LUT_Generator {
    public static void main(String[] args) {
        float limit = 0;
        float step = 0.015f;
        int count = 0;

        int functionSelect = 0;
        String sineLUT = "sine.txt";
        String tanhLUT = "tanh.txt";
        String expLUT = "exp.txt";

        if(args.length > 0) {
            functionSelect = Integer.parseInt(args[0]);
            step = Float.parseFloat(args[1]);
        } 
        
        try {
            FileWriter writer = null;

            switch (functionSelect) {
                case 0:
                    limit = (float) (2*Math.PI);
                    writer = new FileWriter(sineLUT);

                    for(float i = 0; i <= limit; i += step) {
                        writer.write(Integer.toString((int)(Math.sin(i)*32767)));
                        writer.write(", ");

                        count++;
                    }
                break;

                case 1:
                    limit = 3;
                    writer = new FileWriter(tanhLUT);

                    for(float i = 0; i <= limit; i += step) {
                        writer.write(Integer.toString((int)(Math.tanh(i)*32767)));
                        writer.write(", ");

                        count++;
                    }
                break;

                case 2:
                    limit = 1;
                    writer = new FileWriter(expLUT);

                    for(float i = 0; i < limit; i += step) {
                        writer.write(Integer.toString((int)( (Math.pow(10, i) - 1) / 9 * 32767)));
                        writer.write(", ");

                        count++;
                    }
                break;
            
                default:
                    System.out.println("Wrong function selection");
                break;
            }

            writer.write("\n \n (" + count + " samples)");

            writer.close();

        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }
}