LaPosteReleve
=============

Un script python pour regrouper les infos de releves au format CSV de la banque postale


L'application web de la banque postale permet de consulter vos comptes par internet.
Vous pouvez y voir les montants en crédit/débit, en Euro et en Francs...

Malheureusement, cette interface vous affiche seulement un nombre limité de l' activité de votre compte (quelques semaines au lieu de toute l'activité à partir du début de l' année).

Pour avoir un suivi de l'activité de votre compte à long terme, il est possible de télécharger l activité courante de votre compte
en un fichier au format CSV mais avoir plusieurs dizaines de fichiers CSV (pas très lisible) n'est pas exploitable pour une vision globale ou pour une recherche efficace d' une information

LaPosteReleve est un script Python qui, grâce à cette liste de relevés au format CSV, génere un fichier HTML (LaPosteReleve.html)
regroupant les infos de tous les fichiers CSV avec une possiblité de recherche, le montant total en débit/crédit par mois...

Mode d'emploi:
- Créer un répertoire _LaPoste_
- Copier le script _LaPosteReleve.py_ dans ce répertoire _LaPoste_
- Copier tous les fichiers CSV que vous avez téléchargés au fil du temps dans ce répertoire _LaPoste_
- `cd LaPoste`
- `chmod u+x LaPosteReleve.py`
- `./LaPosteReleve.py`

Dans le répertoire LaPoste, vous trouverez un fichier _LaPosteReleve.html_ avec tous vos relevés regroupés
