using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace VizingAlgorithmCSTester
{
    class Program
    {
        [DllImport("E:\\Programming Works\\Vizing's Algorithm\\Vizing's Algorithm\\VizingAlgorithmAnimation\\Vizing's Algorithm\\Bin\\Vizing's Algorithm.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void getGraphColorJson(int[] input, int size);

        static void Main(string[] args)
        {
            int[] temp = new int[14] { 4,6,1,2,1,3,1,4,2,3,2,4,3,4 };
            getGraphColorJson(temp, temp.Length);
            Console.ReadKey();
        }
    }
}
