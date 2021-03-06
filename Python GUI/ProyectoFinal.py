# -*- coding: utf-8 -*-
# -*- coding: 850 -*-
# -*- coding: cp1252 -*-
#!/usr/bin/env python

# Imports
import time
import sys
import serial
from Tkinter import *
import tkMessageBox
import ttk

grado = u"\u2103"
marca = u"\u2122"

def imprimir():             #Integrantes
    tkMessageBox.showinfo("Integrantes", "Sebastian Hernandez\n\r"+"Cristian Camilo Porras")


    
def Info():                 #INFORMACIÓN#
    tkMessageBox.showinfo("Sensores de Nivel Inteligentes", "PSoC 4" + marca+" CY8CKIT-049 42xx\n\r"+"Sensor HC - SR04\n\r"+ "Sensor LM35\n\r" + "Python 2.7\n\r")
		
def comunicar():            #Comunicación#

    def Exit():             #Salir 
                ventana1.deiconify()                        # Muestra la ventana principal
                ventana.withdraw()                          # Oculta una ventana secundaria

    def imprimir_fecha():
                        return str(time.localtime()[2]) + "/" + str(time.localtime()[1]) + "/" + str(time.localtime()[0]) + ", " + str(time.localtime()[3]) + ":" + str(time.localtime()[4]) + ":" + str(time.localtime()[5])

    def refresh_fecha():
                        ventana.after(1000,refresh_fecha)
                        mifecha.set(imprimir_fecha())
    
    a = port.get()
    puerto = a-1
 
    try:
        ser = serial.Serial(puerto, 115200, timeout=1.0)
        #ser.open()
        
        # Clase pricipal en Tkinter
        class Application(Frame):

                  #Funcion que obtiene los datos de los sensores
                  def measure(self):

                            # leemos hasta que encontarmos el final de linea
                            data = ser.readline()

                            # If the answer is not empty, process & display data
                            if (data != ""):
                                      processed_data = data.split(",")

                                      self.t1_data.set("{0:.2f}".format(((float(processed_data[0]))/45.5)*100))#Dato Nivel sensor 1#
                                      self.t1.grid(column=0, columnspan=1,row=1,rowspan=1)
                                      self.t1_data1.set("{0:.2f}".format(((float(processed_data[0]))/45.5)*71.57))#Dato Volumen Sensor 1
                                      self.t11.grid(column=1, columnspan=1,row=1,rowspan=1)
                                      self.t1_data2.set("{0:.2f}".format(float(processed_data[0])))#Dato de Distancia Sensor 1
                                      self.t12.grid(column=2, columnspan=1,row=1,rowspan=1)

                                      self.temp1_data.set("{0:.1f}".format(float(processed_data[1])))#Temperatura sensor 1#
                                      self.temp1.grid(column=3, columnspan=1,row=1,rowspan=1)
                                      
                                      self.t2_data.set("{0:.2f}".format(((float(processed_data[2]))/42.93)*100))#Dato Nivel sensor 2#
                                      self.t2.grid(column=0, columnspan=1,row=2,rowspan=1)
                                      self.t2_data1.set("{0:.2f}".format(((float(processed_data[2]))/42.93)*103.68))#Dato Volumen Sensor 2
                                      self.t21.grid(column=1, columnspan=1,row=2,rowspan=1)
                                      self.t2_data2.set("{0:.2f}".format(float(processed_data[2])))#Dato de Distancia Sensor 2
                                      self.t22.grid(column=2, columnspan=1,row=2,rowspan=1)

                                      self.temp2_data.set("{0:.1f}".format(float(processed_data[3])))#Temperatura sensor 2#
                                      self.temp2.grid(column=3, columnspan=1,row=2,rowspan=1)
     
                                      self.t3_data.set("{0:.2f}".format(((float(processed_data[4]))/26.5)*100))#Dato sensor 3#
                                      self.t3.grid(column=0, columnspan=1,row=3,rowspan=1)
                                      self.t3_data1.set("{0:.2f}".format(((float(processed_data[4]))/26.5)*53.56))
                                      self.t31.grid(column=1, columnspan=1,row=3,rowspan=1)
                                      self.t3_data2.set("{0:.2f}".format(float(processed_data[4])))
                                      self.t32.grid(column=2, columnspan=1,row=3,rowspan=1)

                                      self.temp3_data.set("{0:.1f}".format(float(processed_data[5])))#Temperatura sensor 3#
                                      self.temp3.grid(column=3, columnspan=1,row=3,rowspan=1)
                           
                            self.after(50,self.measure) # Ejecucion de Funcion Comunicar

                    
                  # Funcion de elementos de la interfaz o GUI
                  def createWidgets(self):

                            hora=Label(ventana,textvar=mifecha,font=(16)).place(x=290,y=365)
                                
                            #Impresiones Sensor Tanque 1#
                                                        
                            self.t1 = Label(self,text="Nivel",textvariable=self.t1_data,  font=('Verdana', 14, 'bold'))
                            self.t1.grid(column=0, columnspan=1, ipady=5, ipadx=10, padx=(10,10), pady=(5,5), row=1,rowspan=1)
                            self.t11 = Label(self, textvariable=self.t1_data1, font=('Verdana', 14, 'bold'))
                            self.t11.grid(column=1, columnspan=1, ipady=5, ipadx=10, padx=(10,10), pady=(5,5), row=1,rowspan=1)
                            self.t12 = Label(self, textvariable=self.t1_data2, font=('Verdana', 14, 'bold'))
                            self.t12.grid(column=2, columnspan=1, ipady=5, ipadx=10,padx=(10,10), pady=(5,5), row=1,rowspan=1)
                            
                            self.temp1 = Label(self, textvariable=self.temp1_data, font=('Verdana', 14, 'bold'))
                            self.temp1.grid(column=3, columnspan=1,  ipady=5, ipadx=10, padx=(10,50), pady=(5,5), row=1,rowspan=1)

                            pbar_det1 = ttk.Progressbar(self, orient="vertical", length=80, mode="determinate", variable=self.t1_data2, maximum=46)
                            pbar_det1.grid(column=4, columnspan=1, ipady=5, ipadx=40, padx=(30,30), pady=(10,10), row=1,rowspan=1, sticky=E+W+N+S)

                            valuet1 = Label(ventana, text ="Tanque 1", font=('Verdana', 12, 'bold')).place(x=150,y=20)
                            porcentaje1 = Label(ventana, text ="Nivel").place(x=40,y=70)
                            volumen1 = Label(ventana, text ="Volumen Lts").place(x=128,y=70)
                            Distancia1 = Label(ventana, text ="Distancia").place(x=240,y=70)
                            Temperatura1 = Label(ventana, text=grado, font=('Verdana', 12, 'bold')).place(x=350,y=70)
                            
                            #Impresiones Sensor Tanque 2#
                            
                            self.t2 = Label(self, textvariable=self.t2_data, font=('Verdana', 14, 'bold'))
                            self.t2.grid(column=0, columnspan=1, ipady=5, ipadx=10,padx=(10,10), pady=(5,5),row=2,rowspan=2)
                            self.t21 = Label(self, textvariable=self.t2_data1, font=('Verdana', 14, 'bold'))
                            self.t21.grid(column=1, columnspan=1, ipady=5, ipadx=10,padx=(10,10), pady=(5,5),row=2,rowspan=1)
                            self.t22 = Label(self, textvariable=self.t2_data2, font=('Verdana', 14, 'bold'))
                            self.t22.grid(column=2, columnspan=1, ipady=5, ipadx=10,padx=(10,10), pady=(5,5),row=2,rowspan=1)
                            
                            self.temp2 = Label(self, textvariable=self.temp2_data, font=('Verdana', 14, 'bold'))
                            self.temp2.grid(column=3, columnspan=1, ipady=5, ipadx=10,padx=(10,50), pady=(5,5),row=2,rowspan=1)

                            pbar_det2 = ttk.Progressbar(self, orient="vertical", length=100, mode="determinate", variable=self.t2_data2, maximum=43)
                            pbar_det2.grid(column=4, columnspan=2,ipady=15, ipadx=60, padx=(30,30), pady=(15,10),row=2,rowspan=1, sticky=E+W+N+S)

                            valuet2 = Label(ventana, text ="Tanque 2", font=('Verdana', 12, 'bold')).place(x=150,y=145)
                            porcentaje2 = Label(ventana, text ="Nivel").place(x=40,y=210)
                            volumen2 = Label(ventana, text ="Volumen Lts").place(x=128,y=210)
                            distancia2 = Label(ventana, text ="Distancia").place(x=240,y=210)
                            Temperatura2 = Label(ventana, text =grado, font=('Verdana', 12, 'bold')).place(x=350,y=210)
                            
                            #Impresiones Sensor Tanque 3#
                            
                            self.t3 = Label(self, textvariable=(self.t3_data), font=('Verdana', 14, 'bold'))
                            self.t3.grid(column=0, columnspan=1, ipady=5, ipadx=10,padx=(10,10), pady=(5,5),row=3,rowspan=1)
                            self.t31 = Label(self, textvariable=self.t3_data1, font=('Verdana', 14, 'bold'))
                            self.t31.grid(column=1, columnspan=1, ipady=5, ipadx=10,padx=(10,10), pady=(5,5),row=3,rowspan=1)
                            self.t32 = Label(self, textvariable=self.t3_data2, font=('Verdana', 14, 'bold'))
                            self.t32.grid(column=2, columnspan=1, ipady=5, ipadx=10,padx=(10,10), pady=(5,5),row=3,rowspan=1)
                                                    
                            self.temp3 = Label(self, textvariable=self.temp3_data, font=('Verdana', 14, 'bold'))
                            self.temp3.grid(column=3, columnspan=1, ipady=5, ipadx=10,padx=(10,50), pady=(5,5),row=3,rowspan=1)

                            pbar_det3 = ttk.Progressbar(self, orient="vertical", length=70, mode="determinate", variable=self.t3_data2, maximum=27)
                            pbar_det3.grid(column=4, columnspan=1, ipady=10, ipadx=20,padx=(30,30), pady=(15,10),row=3,rowspan=1, sticky=E+W+N+S)

                            valuet3 = Label(ventana, text ="Tanque 3", font=('Verdana', 12, 'bold')).place(x=150,y=285)
                            porcentaje2 = Label(ventana, text ="Nivel").place(x=40,y=340)
                            volumen2 = Label(ventana, text ="Volumen Lts").place(x=128,y=340)
                            distancia2 = Label(ventana, text ="Distancia").place(x=240,y=340)
                            Temperatura3 = Label(ventana, text=grado, font=('Verdana', 12, 'bold')).place(x=350,y=340)

                  # Inicializacion de variables e  inicio de Comunicacion
                  def __init__(self, master=None):
                            Frame.__init__(self, master)
                            
                            #Variables sensor 1#
                            self.t1_data = StringVar()
                            self.t1_data1 = StringVar()
                            self.t1_data2 = StringVar()
                            self.temp1_data = StringVar()
                            
                            #Variables sensor 2#
                            self.t2_data  = StringVar()
                            self.t2_data1 = StringVar()
                            self.t2_data2 = StringVar()
                            self.temp2_data  = StringVar()

                            #Variables sensor 3#
                            self.t3_data = StringVar()
                            self.t3_data1 = StringVar()
                            self.t3_data2 = StringVar()
                            self.temp3_data = StringVar()
                            
                            self.grid()
                            self.createWidgets()
                            self.measure()
                            
        #Creacion de la ventana Secundaria
        mifecha = StringVar()
        
        ventana = Toplevel(ventana1)
        ventana1.withdraw()                                                         # Oculta una ventana principal
        ventana.protocol("WM_DELETE_WINDOW", "onexit")                              # Elimina la opción de salir para evitar el error
        ventana.title("Sensores de Nivel")
        ventana.geometry("670x530")
        
        imagenI2 = PhotoImage(file="tecnologica.gif")
        ImgLabel2= Label(ventana, image=imagenI2).place(x=0,y=390)
        
        Exit = Button(ventana, text="Salir",command=Exit).place(x=320,y=400)
        refresh_fecha()

        app = Application(master=ventana)
        app.mainloop()
    
    except serial.SerialException:
           tkMessageBox.showwarning("Error", "Error al abrir \n" + "Puerto COM"+str(a))
           
# Creacion de la ventana Principal
ventana1 = Tk()
ventana1.geometry("300x360")
ventana1.title("Proyecto de Grado")
ventana1.resizable(0,0)                                             # Evita que la ventana se pueda cambiar de tamaño

imagenI = PhotoImage(file="Integra2.gif")
ImgLabel= Label(ventana1, image=imagenI).place(x=30,y=130)

port = IntVar()

#Entorno Ventana Principal#

nompro1=  Label(ventana1, text="DISEÑO E IMPLEMENTACIÓN", font=('Verdana', 11, 'bold')).place(x=23,y=10)
nompro2=  Label(ventana1, text="DE SENSORES INTELIGENTES DE ",font=('Verdana', 11, 'bold')).place(x=10,y=30)
nompro3=  Label(ventana1, text="NIVEL PARA PLANTA DE", font=('Verdana', 11, 'bold')).place(x=50,y=50)
nompro4=  Label(ventana1, text="PROCESOS ANÁLOGOS (PPA)",font=('Verdana', 11, 'bold')).place(x=25,y=70)

puerto = Label(ventana1, text="Puerto COM").place(x=10,y=110)
caja1 = Spinbox(ventana1, from_=1, to=24,textvariable=port).place(x=130,y=110)

Enviar= Button(ventana1, text="Comunicar",command=comunicar).place(x=120,y=250)
boton = Button(ventana1, text="Integrantes",command=imprimir).place(x=120,y=280)
info = Button(ventana1, text="Info",command=Info).place(x=138,y=310)

ventana1.mainloop()

