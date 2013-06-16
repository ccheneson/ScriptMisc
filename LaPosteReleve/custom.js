$(function () {
	

    $(".month_name").each(function(i, el) {
		var currMonth = $(this).text();
		var plusTotal = 0;
		var minusTotal = 0;
		var price_current_row = $(this).siblings(".amountEuro").text().replace(",",".");	
		if ($(this).siblings(".amountEuro").hasClass("plus") == true) {
			plusTotal += parseFloat(price_current_row);
		}
		else {
			minusTotal += parseFloat(price_current_row);
		}
		
		$(this).parent().nextUntil(".month").each(function(i, el) {
			var amount = $(el.cells[3]).text().replace(",",".");
			if ($(el.cells[3]).hasClass("plus") == true) {
				plusTotal += parseFloat(amount);
			}
			else if ($(el.cells[3]).hasClass("minus") == true) {
				minusTotal += parseFloat(amount);
			}
		});
		$(this).append("<br />+" + plusTotal.toFixed(2) + "<br />" + minusTotal.toFixed(2));
	});
});
