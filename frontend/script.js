document.addEventListener("DOMContentLoaded", function () {
    const zones = {
        "zone1": document.getElementById("status-zone1"),
        "zone2": document.getElementById("status-zone2"),
        "zone3": document.getElementById("status-zone3"),
        "zone4": document.getElementById("status-zone4"),
        "zone5": document.getElementById("status-zone5")
    };

    function fetchAlerts() {
        fetch("http://localhost:5000/alerts")
            .then(response => response.json())
            .then(data => {
                zones.zone1.textContent = data.zone1 || "No Alerts";
                zones.zone2.textContent = data.zone2 || "No Alerts";
                zones.zone3.textContent = data.zone3 || "No Alerts";
                zones.zone4.textContent = data.zone4 || "No Alerts";
                zones.zone5.textContent = data.zone5 || "No Alerts";
            })
            .catch(error => console.error("Error fetching alerts:", error));
    }

    setInterval(fetchAlerts, 5000);
});
