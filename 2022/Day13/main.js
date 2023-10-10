/*
  L'input représente une liste de paires de paquets sous forme de tableau, chaque paire est séparée par une ligne vide.
  Dans la partie 1, le but est de comparer les paires pour déterminer si elles sont dans le bon ordre.
  Le processus de comparaison des paires est le suivant: On parcourt le tableau des paires en comparant les valeurs comme suit:
    - Si les deux valeurs sont des nombres:
      - Si le nombre1 est supérieur à nombre2, les entrées ne sont pas dans le bon ordre.
      - Si le nombre1 est égal à nombre2, on passe à l'index suivant.
      - Si le nombre1 est inférieur à nombre2, les entrées sont dans le bon ordre.
    - Si les deux valeurs sont des listes:
      - On compare les deux listes en suivant la même procédure que décrite ci-dessus.
      - Si, pendant le parcours, la liste1 manque d'éléments en premier, les entrées sont dans le bon ordre.
      - Si la liste de droite manque d'éléments en premier, les entrées ne sont pas dans le bon ordre.
    - Si une seule valeur est un tableau, transformez la valeur qui ne l'est pas en tableau. Par exemple: "[0,0,0] et 2" devient "[0,0,0] et [2]".
  La réponse attendue est la somme des indices des paires qui sont dans le bon ordre.
  Dans la partie 2, nous devons ajouter deux paquets "[[2]]" et "[[6]]" et trier l'intégralité des paquets par rapport à la comparaison décrite ci-dessus.
  Le résultat attendu est le produit des indices des deux paquets que nous venons d'ajouter dans la liste triée.
*/
const fs = require("fs");
const readline = require("readline");

const path = "input.txt";

let sum = 0;
let indexPair = 1;

const packetList = [];

let PAIR1 = "";
let PAIR2 = "";

const lectureInterface = readline.createInterface({
  input: fs.createReadStream(path),
  output: process.stdout,
  terminal: false,
});

lectureInterface.on("line", (ligne) => {
  if (PAIR1.length == 0) {
    PAIR1 = ligne;
  } else if (PAIR2.length == 0) {
    PAIR2 = ligne;
    processPair(PAIR1, PAIR2);
    packetList.push(PAIR1, PAIR2);
  } else {
    PAIR1 = "";
    PAIR2 = "";
  }
});

lectureInterface.on("close", () => {
  console.log("Part1:", sum);

  packetList.push("[[2]]", "[[6]]");

  packetList.sort((a, b) => {
    const value = processPair(a, b);
    if (value) {
      return -1;
    }
    return 1;
  });

  const index1 = packetList.indexOf("[[2]]") + 1;
  const index2 = packetList.indexOf("[[6]]") + 1;

  console.log("Part2:", index1 * index2);
});

function processPair(pair1, pair2) {
  const arr1 = parseString(pair1)[0];
  const arr2 = parseString(pair2)[0];

  const value = compareArray(arr1, arr2);

  if (value) {
    sum += indexPair;
  }
  indexPair++;

  return value;
}

function compareArray(arr1, arr2) {
  //Parcourir les 2 tableaux jusqu'à arr1.length
  for (let i = 0; i < arr1.length; i++) {
    //Si arr1 est plus grand qu'arr2, return false
    if (arr2.length > i) {
      //Si les deux valeurs sont des nombres
      if (!Array.isArray(arr1[i]) && !Array.isArray(arr2[i])) {
        //Continuer si les deux valeurs sont les mêmes
        if (arr1[i] == arr2[i]) {
          continue;
        }
        //Si arr1[i] est inférieur à arr2[i], alors le paquet est dans le bon ordre
        else if (arr1[i] < arr2[i]) {
          return true;
        }
        //Sinon le paquet n'est pas dans le bon ordre
        return false;
      }
      //L'un des deux est une liste
      else {
        //Transformez en liste si l'une des valeurs n'est pas un tableau
        if (!Array.isArray(arr2[i])) {
          arr2[i] = [arr2[i]];
        } else if (!Array.isArray(arr1[i])) {
          arr1[i] = [arr1[i]];
        }
        const value = compareArray(arr1[i], arr2[i]);
        if (value == null) {
          continue;
        }
        return value;
      }
    } else {
      return false;
    }
  }
  //Les tableaux sont les mêmes car nous sommes sortis de la boucle de comparaison et leur longeur sont égale.
  if (arr1.length == arr2.length) {
    return null;
  }
  //arr1 est plus petit qu'arr2
  return true;
}

function parseString(s) {
  const arr = [];
  let index = 0;
  //Parcourir la chaine
  while (index < s.length) {
    //Si le caractère est une ouverture de crochet
    if (s[index] == "[") {
      let count = 1;
      let indexEnd = index + 1;
      //Chercher la fermeture des crochets
      while (count > 0) {
        if (s[indexEnd] == "[") {
          count++;
        } else if (s[indexEnd] == "]") {
          count--;
        }
        indexEnd++;
      }
      _s = s.substring(index + 1, indexEnd - 1);
      arr.push(parseString(_s)); //Appel récursif de la fonction avec la sous chaine trouvée au dessus et ajout du résultat a l'array
      index = indexEnd + 1;
    } /* Si on trouve une virgule, on skip */ else if (s[index] == ",") {
      index++;
    } /* On a trouvé un nombre */ else {
      const commaIndex = s.indexOf(",", index);
      //Si plus de virgule dans la chaine, nous sommes à la fin, on ajoute le nombre a l'array
      if (commaIndex == -1) {
        const number = parseInt(s.substring(index, s.length));
        if (!isNaN(number)) {
          arr.push(number);
        }
        break;
      }
      //Sinon on ajoute le nombre a l'array et on continue
      else {
        const number = parseInt(s.substring(index, commaIndex));
        arr.push(number);
        index = commaIndex + 1;
      }
    }
  }
  return arr;
}
