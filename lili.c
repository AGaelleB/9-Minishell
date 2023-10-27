Agir en fonction du signal dans votre logique principale : 
Là où vous appelez write_in_fd, vous devez vérifier si -1 
est retourné et ne pas exécuter le code suivant si 
l'utilisateur a appuyé sur CTRL+C.




// ... code précédent
int result = write_in_fd(fd, delimiter, current);
if (result == -1) {
    // Ne pas exécuter 'ls' ou la commande suivante si CTRL+C a été appuyé
    // Peut-être revenir à la boucle principale de votre shell
    return (-1);
}
// ... reste du code
