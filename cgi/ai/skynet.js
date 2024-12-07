
function fadeIn() {
	const bubble = document.getElementById('bubble');
	const text = document.getElementById('result');
	let opacity = parseFloat(window.getComputedStyle(bubble).opacity);
  
	const fadeEffect = setInterval(() => {
    if (opacity < 1) {
		opacity += 0.05; 
		bubble.style.opacity = opacity;
		text.style.opacity = opacity;
	} else {
		clearInterval(fadeEffect);
	}
	}, 100);
}
const form = document.getElementById('myForm');
const resultDiv = document.getElementById('result');
const bubble = document.getElementById('bubble_cont');
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
				bubble.style.display = "block";
				
				// Défile vers le bas de la page
			
				// setTimeout(() => {
				// 	window.scrollTo({
				// 	  top: document.body.scrollHeight,
				// 	  behavior: 'smooth'
				// 	}, 1);
				// void document.body.offsetHeight;

// Maintenant, scrolle vers le bas
window.scrollTo({
  top: "1000", // Définit la position tout en bas
  behavior: 'smooth'
});
				fadeIn();
            })
	        
            .catch((error) => {
                console.error('Erreur lors de la requête :', error);
                resultDiv.innerHTML = `<p style="color: red;">Une erreur s'est produite : ${error.message}</p>`;
            });
    });
});


  