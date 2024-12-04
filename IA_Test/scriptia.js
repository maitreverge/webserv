// Sélection du formulaire, de la zone de résultat, et des boutons

function fadeIn() {
	const element = document.getElementById('bubble');
	const text = document.getElementById('result');
	element.style.display = 'block'; // Rendre visible si c'était caché
	let opacity = parseFloat(window.getComputedStyle(element).opacity);
  
	const fadeEffect = setInterval(() => {
	  if (opacity < 1) {
		opacity += 0.05; // Augmente l'opacité
		element.style.opacity = opacity;
		text.style.opacity = opacity;
	  } else {
		clearInterval(fadeEffect); // Arrête l'effet à l'opacité maximale
	  }
	}, 100);
}
const form = document.getElementById('myForm');
const resultDiv = document.getElementById('result');
const buttons = form.querySelectorAll('button[type="submit"]');
// Ajout d'un écouteur d'événement à chaque bouton
buttons.forEach((button) => {
    button.addEventListener('click', (event) => {
        // Empêche le rechargement de la page
        event.preventDefault();

        // Récupère l'URL spécifique au bouton
        const formAction = button.getAttribute('formaction');

        // Récupération des données du formulaire
        const formData = new FormData(form);

        // Envoi de la requête POST avec la bonne URL
        fetch(formAction, {
            method: 'POST',
            body: formData,
        })
            .then((response) => {
                if (!response.ok) {
                    throw new Error(`Erreur HTTP : ${response.status}`);
                }
                return response.text(); // ou response.json() si tu attends un JSON
            })
            .then((result) => {
                // Affiche le résultat dans la div
                resultDiv.innerHTML = result;
				fadeIn();
            })
            .catch((error) => {
                console.error('Erreur lors de la requête :', error);
                resultDiv.innerHTML = `<p style="color: red;">Une erreur s'est produite : ${error.message}</p>`;
            });
    });
});


  