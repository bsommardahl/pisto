
module Product = {

    type product = {
        id: string,
        name: string,
        tags: array(string),
        suggestedPrice: float    
    };

    let products: array(product) = [|
        {
            id: "Americano15",
            name: "Americano 15",
            tags: [|"Cafes"|],
            suggestedPrice: 15.00
        },
        {
            id: "Americano20",
            name: "Americano 20",
            tags: [|"Cafes"|],
            suggestedPrice: 20.00
        },
    |];
};

module Order = {

    type orderItem = {
        product: Product.product,
        addedOn: int,    
        salePrice: int
      };
    
    type order = {
        id: string,
        customerName: string,
        amountPaid: float,    
        orderItems: array(orderItem),
        paymentTakenBy: string,
        paidOn: int,
        createdOn: int
      };    
};
