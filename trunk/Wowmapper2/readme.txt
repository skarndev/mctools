Wow Mapper beta
ce programme sert � visualiser les portions de minimap et � les assembler en region telles que definies dans le jeu World of Warcraft.

Indiquez le chemin de world of warcraft dans le premier champ appropri� puis cliquer sur initialiser.

-Rendu des cartes exterieures: azeroth, kalimdor etc, indiqu�e en blanc:
Selectionner une r�gion puis cliquer sur g�n�rer. 
Cela va cr�er une preview de la map de la r�gion.
Pour faire un rendu haute definition, choisir la resolution des cellules. Selectionner en faisant glisser la souris la zone � dessiner (un rectangle jaune translucide marque la s�lection)
Attention les grandes cartes peuvent generer des fichiers de tres grande tailles (+ de 300mo en bmp). Le processus d'enregistrement peut etre long selon les configurations
le format de sortie est en bitmap 32 bit (*.bmp). ou Jpg 24 bit (*.jpg).

-Rendu des cartes de zones "int�rieures" (donjon ou villes etc...), indiqu�es en gris�:
Il faut selectionner une image dans l'arborscence du r�pertoire concern� afin de d�finir quelle portion exacte doit etre rendue, car plusieurs cartes peuvent etre r�unies sous le meme groupe.
Par exemple, pour g�n�rer la carte de Molten core, il faut ouvrir l'arborescence de AZ_blackrock (repertoire global contenant toutes les instances de blackrock), puis selectionner une des images de la famille "Blackrock_lower_guild". Pour le rendu des lbrs-Ubrs, il faudra en revanche choisir une image de la famille "Blackrock_upper_instance".
Lancer une preview, afin de pr�parer la carte, puis dans l'onglet de visualisation, il est possible de jouer avec les curseurs pour modifier l'echelle, et l'apparition des blocs en fonction de leur altitude. (Le curseur zbufferIn fait disparaitre les blocs par le dessus et le curseur zbufferOut par le dessous).
le rendu final d'export prend en compte les parametres de zbuffer en cours et d'�chelle. La couleur de fond est volontairement satur�e de facon � faciliter le post-traitement.

Ce programme se sert des librairies:
- packCraft.dll: credits � Linghuye - site http://www.cnitblog.com/linghuye
- blp2decode.dll: credits � Technetium, site web ?
- Le composant virtualtreeview utilis� est disponible sur http://www.delphi-gems.com/VirtualTreeview/VT.php. Ce composant est copyright 1999 2007 Mike Mischke.
- l'excellente librairie Graphic G32. Cette librairie est copyright 2000 2006 Alex A. Denisov est disponible ici: http://www.g32.org
- la libraire ijl15.dll , intel jpg library.

Wow mapper est copyright Tixu 2004-2007.

License:
Installer et utiliser wow mapper signifie que vous acceptez les termes et les conditions de sa license.
Wow mapper est fourni en tant que Freeware pour un usage strictement non commercial.
Vous avez le droit d'utiliser et faire un nombre illimit� de copie de ce logiciel pour un usage priv�. La diffusion par voie de presse est interdite, sauf demande expresse. 
Ce logiciel est fourni "TEL QUEL", et aucune garantie expresse ou implicite ne sont accord�es.
L'auteur d�cline toute responsabilit� pour tout dommage direct ou indirect, y compris notamment perte de profits ou d'�conomies escompt�es ou de tout dommage incident, sp�cial ou de tout autre dommage financier ou commercial ou pr�judice moral, quelle qu'en soit la cause.


World of Warcraft est copyright Blizzard entertainment 2004-2007



-------------historique----------------
01.01.2007 Support de la g�n�ration de cartes des donjons et autres zones int�rieures. Ajout d'une recherche simplifi�e
17.09.2006 Mise � jour pour Burning crusade, via la librairie PackCraft.dll (non distribu�e)
15.11.2004 Suppression d'un memory leak
14.11.2004 meilleure gestion des masques alpha des images affich�es
13.11.2004 premiere version distribu� (beta)

