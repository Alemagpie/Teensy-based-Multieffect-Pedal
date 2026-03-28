public class msToQ15 {
    public static void main(String[] args) {
        int audioRate = 44100;
        double time_ms = 0;
        double result = 0;

        if(args.length > 0) {
            time_ms = Float.parseFloat(args[0]);
        } 
        
        result = (1 - Math.exp(- 1 / (audioRate * time_ms / 1000))) * 32767.0f;

        System.out.println("Q15 coefficient of " + time_ms + "ms is: " + result);
    }
}
