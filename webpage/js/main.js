async function fetchData() {
    try {
        const response = await fetch('/data/data.txt');
        
        // Check if the response status is OK (status code 200-299)
        if (!response.ok) {
            throw new Error(`Network response was not ok (status: ${response.status})`);
        }

        // Fetch the text content of data.txt
        const text = await response.text();
        
        // Split text into lines, trim any whitespace, and filter out empty lines
        const values = text.trim().split('\n').filter(line => line.length > 0);

        // Display the latest value or all values, depending on your preference
        const latestValue = values[values.length - 1];
        document.getElementById("data-container").textContent = `Current Value: ${latestValue}`;

        // Optionally display all values in a list format (comment this out if only the latest is needed)
        // document.getElementById("data-container").innerHTML = values.map(val => `<div>${val}</div>`).join('');

    } catch (error) {
        // Display error message to user and log to console
        document.getElementById("data-container").textContent = "Error loading data.";
        console.error("Error fetching data:", error);
    }
}

// Set up polling every 1 second
setInterval(fetchData, 1000);
