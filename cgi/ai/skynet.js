
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
	}, 80);
}

let wait;
let angle = 0;
function waiting() {
	const ai = document.getElementById('ai');
		
	let flag = false;
	wait = setInterval(() => {
		if (!flag && angle < 2) {
			angle += 0.1; 
			ai.style.transform = 'rotateZ(' + angle + 'deg)';
			
		} else {
			flag = true;	
			angle -= 0.1; 
			ai.style.transform = 'rotateZ(' + angle + 'deg)';
		}
		if (angle < 0)
			flag = false;
	}, 10);	
}

function bubbleAppear() {
	bubble.style.display = "inline-block";		
	window.scrollTo({
		top: "1000", 
		behavior: 'smooth'
	});
	fadeIn();
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

		waiting();	
        fetch(formAction, {
            method: 'POST',
            body: formData,
        })
            .then((response) => {
                if (!response.ok) {
                    throw new Error(`Error HTTP : ${response.status}`);
                }
                return response.text();
            })
            .then((result) => {
				clearInterval(wait);               
                resultDiv.innerHTML = result;
				bubbleAppear();
            })	        
            .catch((error) => {
				clearInterval(wait);
                console.error('Erreur lors de la requête :', error);				
                resultDiv.innerHTML = `<p style="color: red;">
					${error.message}</p>`;
				bubbleAppear();
            });
    });
});


  