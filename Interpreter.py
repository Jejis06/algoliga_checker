import os

class Interpreter:

    def __init__(self,ARR_SIZE=30,RECURSION_LIMIT=36):

        self.ALPHABET_SIZE = 27


        self.stack = [0] * RECURSION_LIMIT
        self.arr = [0] * ARR_SIZE
        self.out = ""

        self.instruction = 0
        self.mvs = 0
        self.arr_ind = 0

    def compile(self,code):
        self.prog_size = len(code)
        self.code = code
        self.decode(code,0)

    def decode(self,code:str, stack_ind:int):
        temp = 0
        while(self.instruction < self.prog_size):
            self.mvs+=1


            if code[self.instruction] == '>':
                self.arr_ind = (30 + self.arr_ind + 1) % 30

            elif code[self.instruction] == '<':
                self.arr_ind = (30 + self.arr_ind - 1) % 30

            elif code[self.instruction] == '+':
                self.arr[self.arr_ind] = (self.ALPHABET_SIZE + self.arr[self.arr_ind] + 1)%self.ALPHABET_SIZE

            elif code[self.instruction] == '-':
                self.arr[self.arr_ind] = (self.ALPHABET_SIZE + self.arr[self.arr_ind] - 1) % self.ALPHABET_SIZE

            elif code[self.instruction] == '.':

                if self.arr[self.arr_ind] == 0:
                    self.out += ' '
                else:
                    self.out += chr(self.arr[self.arr_ind] + 65 - 1)

            elif code[self.instruction] == '[':
                self.stack[stack_ind + 1] = self.instruction
                self.instruction += 1

                while(self.arr[self.arr_ind] != 0):
                    self.decode(code,stack_ind+1)

                temp = stack_ind + 1
                while (temp >= (stack_ind + 1)) or code[self.instruction] != ']':
                    self.instruction += 1
                    if code[self.instruction] == '[':
                        temp += 1
                        self.stack[temp] = self.instruction
                    elif code[self.instruction] == ']':
                        temp -= 1


            elif code[self.instruction] == ']':
                self.instruction = self.stack[stack_ind] + 1
                return

            self.instruction+=1


        return None

    def result(self):
        return{
            "Moves":self.mvs,
            "Length":self.prog_size,
            "Out":self.out
        }




