#include <iostream>
#include <math.h>


int main()
{
    double V = 1;
    double V0 = 1;
    double b = 3;
    double alpha = 2;
    bool switch1 = false;

    float time = 0;
    float T = 0;

    while(time < 5)
    {
        if(V >= V0)
        {
            if(time - T >= 0.2 || time == 0)
            {
                T = time;
                while(V >= V0 && time < 5)
                {
                    switch1 = false;
                    time += 0.1;
                    V = V*exp(-(time - T)*b);

                    // std::cout << "decreasing " << time << " " << V << std::endl;
                    std::cout << time << " " << V << std::endl;
                }
            }
            else
            {
                while(time - T < 0.2)
                {
                    switch1 = true;
                    time += 0.1; 
                    V = V + alpha*(time - T);

                    // std::cout << "increasing " << time  << " " << V << std::endl;
                    std::cout << time  << " " << V << std::endl;
                }
            }
            
        }

        else
        {
            if(time - T >= 0.2 || time == 0)
            {
                T = time;
                while(time < 5 && V < V0)
                {
                    switch1 = true;
                    time += 0.1; 
                    V = V + alpha*(time - T);

                    // std::cout << "increasing " << time << " " << V << std::endl;
                    std::cout << time << " " << V << std::endl;
                }
            }

            else
            {
                while(time - T < 0.2)
                {
                    switch1 = true;
                    time += 0.1;
                    V = V + alpha*(time - T);

                    // std::cout << "decreasing " << time  << " " << V << std::endl;
                    std::cout << time  << " " << V << std::endl;
                }
            }
        }
    }
}

