# Problem Statement: 
We are given the Bayesian Network created by the researchers. The network is shown below:
![Screenshot from 2024-03-18 22-02-39](https://github.com/Ashwin-996/Bayesian_Leaning/assets/138316564/011614b4-7a18-4f9a-ab40-a74809878723)
Eight diagnoses are modeled here: hypovolemia, left ventricular failure, Anaphylaxis, insufficient
analgesia, pulmonary embolus, intubation, kinked tube, and disconnection. The observable nodes are CVP,
PCWP, History, TPR, Blood Pressure, CO, HR BP, HR EKG, HR SAT, SaO2, PAP, MV, Min Vol, Exp CO2,
FiO2 and Pres.
<br>
<br>
Such networks can be represented in many formats. We will use the .bif format. BIF stands for Bayesian
Interchange Format. The details about the format are here - https://www.cs.cmu.edu/afs/cs/user/fgcozman/www/Research/InterchangeFormat/. A .bif parser is also provided so that you can
start directly from a parsed Bayesian network represented as a graph. 
<br>
## Input Format :
<pre>
We will work with alarm.bif network. Please have a look at this file to get a basic understanding of how
this information relates to the Bayes net image above. A sample Bayes net is as follows
  
variable “X” {
type discrete[2] { “True” “False” };
}
variable “Y” {
type discrete[2] { “True” “False” };
}
variable “Z” {
type discrete[2] { “True” “False” };
}
probability(“X”) { table 0.2 0.8 ; }
probability(“Y”) { table 0.4 0.6 ; }
probability(“Z” “X” “Y”) { table 0.2 0.4 0.3 0.5 0.8 0.6 0.7 0.5; }
  
This says that X, Y, and Z all have two values each. X and Y has no parents and prior P(X=True)=0.2,
P(X=False)=0.8, and so on. Z has both X and Y as parents. Its probability table says P(Z=True|X=True,
Y=True) = 0.2, P(Z=True|X=True, Y=False) = 0.4 and so on.
  
The input network will have the Bayes net structure including variables and parents, but will not have
probability values. We will use -1 to represent that the probability value is unknown.
  
probability(“X”) { table -1 -1 ; } will represent that prior probability of X is unknown and needs to be
computed via learning.
  
To learn these values a data file will be provided. Each line will be a patient record. All features will be
listed in exactly the same order as in the .bif network and will be comma-separated. If a feature value is
unknown we will use the special symbol “?” for it. There will be no more than 1 unknown value per row.
Example:
  
“True”, “False”, “True”
“?”, “False”, “False”
Here the first row says that X=True, Y=False and Z=True. The second row says that X is not known, Y and
Z are both False.
  
Overall your input will be alarm.bif with most probability values -1 and this datafile. The datafile will have
about 10,000 patient records.
</pre>
<br>

## Output Format :
Your output will be the result of learning each probability value in the conditional probability tables. In
other words, you need to replace all -1s with a probability value upto four decimal places. Thus, your
output is a complete alarm.bif network.
<br>
<br>

## Files :
<b>A Dataset file</b>: records.dat file where a single line is a single patient record and each variable in the record
is separated by spaces. The unknown record is marked by “?”. Each line contains at max 1 missing record.
The file contains more than 11000 records. 
<br>
<br>
A <b>format checker</b> to check if the output file adheres to alarm.bif format. The format checker assumes that
alarm.bif, solved_alarm.bif and gold_alarm.bif are present in current directory and outputs its results.
<br>
<br>
<b>alarm.bif</b> whose parameters need to be learned.
<br>
<br>
<b>gold_Alarm.bif</b> has the true parameters.
<br>
<br>
<b>solved_alarm.bif</b> - output file containing learned probabilities.
