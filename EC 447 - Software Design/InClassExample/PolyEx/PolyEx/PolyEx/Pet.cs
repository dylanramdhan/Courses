using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PolyEx
{
    class Pet
    {
        public string size;

        public virtual string Draw()
        {
            return "Error.";
        }
    }

    class Dog : Pet
    {
        public Dog()
        {
            size = "large";
        }

        public override string Draw()
        {
            return "I am a dog. I am a " + size + " pet.";
        }

    }

    class Cat : Pet
    {
        public Cat()
        {
            size = "medium";
        }

        public override string Draw()
        {
            return "I am a cat. I am a " + size + " pet.";
        }

    }

    class Bird : Pet
    {
        public Bird()
        {
            size = "small";
        }

        public override string Draw()
        {
            return "I am a bird. I am a " + size + " pet.";
        }

    }
}
