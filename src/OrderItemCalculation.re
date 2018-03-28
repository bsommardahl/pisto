open OrderData;

type totals = {
  subTotal: int,
  tax: int,
  total: int,
};

type totalCalculator = (int, Order.orderItem) => totals;

let totalFirstCalculator: totalCalculator =
  (taxPercent: int, item: Order.orderItem) => {
    let total = item.salePrice;
    let realSalePrice = total * 100 / (100 + taxPercent);
    let tax = total - realSalePrice;
    {subTotal: realSalePrice, tax, total};
  };

let subTotalFirstCalculator: totalCalculator =
  (taxPercent: int, item: Order.orderItem) => (
    {
      let salePrice = item.salePrice;
      let tax = salePrice * taxPercent / 100;
      let total = salePrice + tax;
      {subTotal: salePrice, tax, total};
    }: totals
  );

let exemptCalculator: totalCalculator =
  (_tax, item: Order.orderItem) => (
    {subTotal: item.salePrice, tax: 0, total: item.salePrice}: totals
  );

let getTotals = (orderItems: list(Order.orderItem)) : totals => {
  let itemTotals =
    orderItems
    |> List.map(item =>
         switch (Order.(item.taxCalculation)) {
         | Tax.TotalFirst(rate) => item |> totalFirstCalculator(rate)
         | Tax.SubTotalFirst(rate) => item |> subTotalFirstCalculator(rate)
         | Tax.Exempt => item |> exemptCalculator(0)
         }
       );
  let subTotal =
    Belt.List.reduce(itemTotals, 0, (acc, totals) => acc + totals.subTotal);
  let tax =
    Belt.List.reduce(itemTotals, 0, (acc, totals) => acc + totals.tax);
  let total =
    Belt.List.reduce(itemTotals, 0, (acc, totals) => acc + totals.total);
  {subTotal, tax, total};
};