// state = setup
// switch_air = 36
// switch_heart = 39
// air_ex = 27
// air_in = 4
// heart = 19

int heart_motor = 19; //fil vert
int air_ex = 27; //fil marr
int air_in = 4; //fil blanc
int switch_air = 36; //fil bleu
int switch_heart = 39; //fil orange

int switch_air_val = 0;
int switch_heart_val = 0;

int n_surveillance = 1;

bool do_massage = true;


int state;
int k; //ce qui est retourné par la fonction send_sms()
int rythm_result; //ce qui est retourné par la fonction rythm()


  
/*
 * State 1: premier appel au samu
 * State 2: rythme
 * State 3: envoi constantes au samu
 * State 4: massage/insufflation
 * State 5: stop
 * State 6: surveillance
 */

void setup() {

   pinMode(air_ex, OUTPUT);
  pinMode(air_in, OUTPUT);
  digitalWrite(air_ex, HIGH);
  digitalWrite(air_in, HIGH);    
  
  Serial.begin(9600);
  Serial.println("Mise sous tension de l'appareil");
  Serial.println("Chargement en cours, veuillez patienter");
  
  pinMode(heart_motor, OUTPUT);

  pinMode(switch_air, INPUT);
  pinMode(switch_heart, INPUT);

 

  state = 1;
}


/********************************************************
 MAIN LOOP
*********************************************************/

void loop() {

if(state != 5){  // On vérifie que la boucle principale à le droit d'être exécuter (c-a-d state != stop)
  
  if(state == 1){  // Premier appel au SAMU
   send_sms(1);
    if(k == true){
      Serial.println("Le SAMU vient d'être informé");
      }  
      
    state = 2;
    }

  if(state == 2){ // Premiere lecture rythme
    rythm();
    if(rythm_result == 1){ //Si le programme détermine qu'il faut masser
      state = 3;
      }
      
    if(rythm_result == 2){ //Si le programme détermine qu'il ne faut pas masser
      state = 6;    
      }

    if(rythm_result == 3){  //Si le programme ne parvient pas à déterminer le rythme cardiaque
      Serial.println("Echec lors de la lecture du rythme cardiaque");
      Serial.println("Confirmez-vous que la personne ne respire pas et est inconsciente? (oui, non)");
      String result_ask = "pas de reponses";
      
      while((result_ask != "oui") && (result_ask != "non")){  //Tant que pas de réponse de l'utilisateur
        
        if(Serial.available() > 0){
           result_ask = Serial.readString();
           result_ask.remove(3);      
           Serial.println(result_ask);
           
           /* int length = result_ask.length();
           Serial.println(length);

           if(result_ask.equals("oui") || result_ask.equals("non")){ // Y pour yes, on masse
            Serial.println("Equivalent à oui ou à non");
           }
      
          else{
            Serial.println("diff");
          } */
        }
      }
      
      if(result_ask.equals("oui")){ // On masse
        Serial.println("Vous avez confirmé que la personne est inconsiente et ne respire pas");
        state = 3;
      }

      if(result_ask == "non"){ // On ne masse pas, on surveille
        Serial.println("La personne respire ou est consciente");
        state = 6;
        }
      
      }
    }

  if(state == 3){   //Envoi constantes au SAMU + alerte début massage
    send_sms(2);
    
    if(k == true){
      Serial.println("Le SAMU vient d'être informé");
      }  
    
    state = 4;
    
    }

  if(state == 4){  //Massage
    Serial.println("Entree dans le mode massage");
//    do_message = true;
    massage();
    state = 6; //Mise en surveillance
    }

  if(state == 6){ //Surveillance
    if(n_surveillance == 1){
      Serial.println("Entrée en mode de surveillance");
      n_surveillance++;
    }
    surveillance();
    }
   
  }

}



/********************************************************
   MASSAGE CARDIAQUE
*********************************************************/

void massage(){

while(do_massage == true){ // Tant qu'il faut effectuer le massage
  int n_heart = 0;
  
  // 30 compressions
  while(n_heart < 30){ 
    
    digitalWrite(heart_motor, HIGH);
    switch_heart_val = digitalRead(switch_heart);
    if(switch_heart_val == HIGH){
      n_heart++; 
      Serial.println(n_heart); 
    }
    delay(70);
  }

  digitalWrite(heart_motor, LOW);

  // 2 insufflations
  int n_air = 0;
  while(n_air < 4){ // 1 aller-retour = 2n
    if(n_air == 0 || n_air == 2){
      digitalWrite(air_ex, LOW);
      digitalWrite(air_in, HIGH);
      delay(300); //Laisse le temps de quitter le fin de course avant mesure des fins de course
    }

    if(n_air == 1 || n_air == 3){
      digitalWrite(air_in, LOW);
      digitalWrite(air_ex, HIGH);
      delay(300); //Pour ne pas lire 2x la même valeur sur le fin de course
    }
    
    switch_air_val = digitalRead(switch_air); //Lecture des fins de course
    
    if(switch_air_val == HIGH){
      n_air++;
      if(n_air == 2 || n_air == 4){
         Serial.println("Nombre d'insufflations:");
         Serial.println(n_air/2);
      }
    }    

  }

  digitalWrite(air_ex, HIGH);
  digitalWrite(air_in, HIGH);        
}
}



/********************************************************
  FONCTION POUR ENVOYER SMS AU SAMU
*********************************************************/

bool send_sms(int n_sms){ //Non fonctionnel, agit comme si le sms a correctement été envoyé
  if(n_sms == 1){ //Premier envoi
    Serial.println("Le système Sierros n°Beta_01 vient d'être déclenché");
    }

  if(n_sms == 2){
    Serial.println("Envoi des constantes vitales et de l'ECG");
    }
    
  Serial.println("Envoi d'un sms au samu");
  bool k = true;
  return k;
  }


  
/********************************************************
 ANALYSE DU RYTHME CARDIAQUE, DETERMINATION SI ASYSTOLIE, FIBRILLATION OU NORMAL
*********************************************************/

int rythm(){
  rythm_result = 3; //Tant que fonction rythm() non fonctionnelle (bypass)
  return rythm_result;
  }



/********************************************************
 SURVEILLANCE DE L'EVOLUTION DE L ETAT DE LA VICTIME
*********************************************************/

bool surveillance(){ //Tant que fonction rythm() non fonctionnelle, surveillance() inutile
  bool surveillance_result;
  return surveillance_result;
  }




  

// Si state != stop

  /* Si state = first_call 
   *  sms "l'appareil n° CONST_NUM_SIERROS va être utilisé"
  */
  
  /* Si state = rythm
   *  print "veuillez placer les electrodes"
   *  si = OK
   *  Analyse du rythme cardiaque en cours. Ne touchez pas la victime.
   *  - determination arret ou non
   *  Massage recommandé / non recommandé
   *  state = first_call
   */
    
  /* Si state = second_call
   *  sms "début massage / pas massage"
   *  si massage, n_boucle = 1, state = main
   *  sinon, surveillance
   */

   /* Si state = main
    *  Boucle 30 compressions
      * n = 0   
      * while n < 30
        * heart = high
        * if (switch_heart = high) { n++; print n_heart;}
    *  2 insufflations
      * n_air = 0  
      * while n_air < 4 (aller retour = 2n)
          * air_ex = high
          * air_in = low
          * delay(50);
          * if (switch_air = high) {air_ex = low; air_in = high}
          * delay(50)
          * if (switch_air = high) {air_in = low; n_air++; print n_air;}
          
    *  rythm()
      *  Si rythme = bad, retour dans la boucle
      *  Si rythme = good, samu sms, state = surveillance
    *  ++n_boucle
    */

    /* Si state = surveillance
     *  print  Analyse du rythme cardiaque en cours. Ne touchez pas la victime.
     *  rythm();
     *  
     *  Si rythm = bad, state = main
     *  delay (2000);
     */

//Sinon "Fin. Les secours prennent le relais"


