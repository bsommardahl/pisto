module Order = {
  type orderItem = {
    product: Product.t,
    addedOn: float,
    salePrice: float,
  };
  type order = {
    id: string,
    customerName: string,
    amountPaid: float,
    orderItems: array(orderItem),
    paymentTakenBy: string,
    paidOn: float,
    createdOn: float,
  };
};