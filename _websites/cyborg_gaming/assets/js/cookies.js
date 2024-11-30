document.addEventListener('DOMContentLoaded', () => {
	// Fonction pour vérifier si un cookie est présent
	function getCookie(name) {
		const cookies = document.cookie.split('; ').reduce((acc, cookie) => {
			const [key, value] = cookie.split('=');
			acc[key] = value;
			return acc;
		}, {});
		return cookies[name];
	}

	const sessionID = getCookie('sessionID');
	const banner = document.getElementById('cookieBanner');
	const connectedInfo = document.getElementById('connectedInfo');

	if (!sessionID) {
		// Si le cookie est absent, affiche la barre
		banner.style.display = 'block';
		connectedInfo.style.display = 'none';

		document.getElementById('acceptCookies').addEventListener('click', () => {
			// Envoie une requête au serveur pour accepter les cookies
			fetch('/accept-cookies?action=accept', {
				method: 'POST',
				headers: {
					'Content-Type': 'application/json',
				},
			}).then(() => {
				window.location.reload();
			});
		});
	} else {
		// Si le cookie est présent, affiche l'état connecté
		banner.style.display = 'none';
		connectedInfo.style.display = 'block';
	}
});
