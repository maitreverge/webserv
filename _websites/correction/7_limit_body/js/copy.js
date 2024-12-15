function copyToClipboard(text, element) {
	const tempInput = document.createElement('input');
	tempInput.value = text;
	document.body.appendChild(tempInput);
	tempInput.select();
	document.execCommand('copy');
	document.body.removeChild(tempInput);

	const originalText = element.innerText;
	element.innerText = 'Copied';
	setTimeout(() => {
		element.innerText = originalText;
	}, 2000);
}