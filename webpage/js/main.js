document.addEventListener('DOMContentLoaded', function() {
    fetchData();
    setInterval(fetchData, 500); // Fetch data every half a second
});

function fetchData() {
    fetch('http://localhost:8080') // Use the correct URL and port
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            document.getElementById('data-container').textContent = `Current: ${data.current}`;
        })
        .catch(error => {
            console.error('There was a problem with the fetch operation:', error);
            document.getElementById('dataDisplay').textContent = 'Failed to load data.';
        });
}
