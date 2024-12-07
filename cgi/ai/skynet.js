
function fadeIn() {
	const bubble = document.getElementById('bubble');
	const text = document.getElementById('result');
	let opacity = parseFloat(window.getComputedStyle(bubble).opacity);
  
	const fadeEffect = setInterval(() => {
    if (opacity < 1) {
		opacity += 0.05; 
		bubble.style.opacity = opacity;
		text.style.opacity = opacity;
	} else
		clearInterval(fadeEffect);	
	}, 100);
}

const form = document.getElementById('myForm');
const resultDiv = document.getElementById('result');
const bubble = document.getElementById('bubble_cont');
const buttons = form.querySelectorAll('button[type="submit"]');

buttons.forEach((button) => {
    button.addEventListener('click', (event) => {  

        event.preventDefault();       
        const formAction = button.getAttribute('formaction');     
        const formData = new FormData(form);
       
        fetch(formAction, {
            method: 'POST',
            body: formData,
        })
            .then((response) => {
                if (!response.ok) {
                    throw new Error(`Erreur HTTP : ${response.status}`);
                }
                return response.text();
            })
            .then((result) => {               
                resultDiv.innerHTML = result;
				bubble.style.display = "block";		
				window.scrollTo({
				top: "1000", 
				behavior: 'smooth'
				});
				fadeIn();
            })	        
            .catch((error) => {
                console.error('Erreur lors de la requête :', error);
                resultDiv.innerHTML = `<p style="color: red;">Une erreur` +
					`s'est produite : ${error.message}</p>`;
            });
    });
});


  